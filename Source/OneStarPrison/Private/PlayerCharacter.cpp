// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
//New Ones
#include "Pickupable.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "PickupableKey.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	RespawnCheckpoint = GetActorLocation();

	CurrentInteractWidget = CreateWidget<UUserWidget>(GetWorld(), InteractWidgetClass);
	CurrentPickupWidget = CreateWidget<UUserWidget>(GetWorld(), PickupWidgetClass);

	cacheArmLength = CameraBoom->TargetArmLength;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, IsInteracting);
	DOREPLIFETIME(APlayerCharacter, CanInteract);
	DOREPLIFETIME(APlayerCharacter, PickedUpItem);
	DOREPLIFETIME(APlayerCharacter, CurrentThrowWidget);
	DOREPLIFETIME(APlayerCharacter, IsHoldingDownThrow);
	DOREPLIFETIME(APlayerCharacter, IsPickingUp);

	DOREPLIFETIME(APlayerCharacter, CanMove);
	DOREPLIFETIME(APlayerCharacter, IsCrouching);

	DOREPLIFETIME(APlayerCharacter, IsClimbing);
	DOREPLIFETIME(APlayerCharacter, ClimbSpeed);

	DOREPLIFETIME(APlayerCharacter, CurrentPickupWidget);
	DOREPLIFETIME(APlayerCharacter, CurrentInteractWidget);

	DOREPLIFETIME(APlayerCharacter, HitByWallCount);

	DOREPLIFETIME(APlayerCharacter, PunchTimer);
	DOREPLIFETIME(APlayerCharacter, PunchDelay);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Punch Delay
	if (PunchTimer <= PunchDelay)
	{
		PunchTimer += DeltaTime;
	}

	ServerCheckPickup();
	ServerCheckInteract();

	//Check if the player is holding down throw
	if (IsHoldingDownThrow)
	{
		if (ThrowPowerScale < MaxThrowPower)
		{
			//Reset the spline component
			//Clear all the spline points
			SplineComponent->ClearSplinePoints(true);
			//Iterate through the spline component array
			for (int i = 0; i < SplineComponentArray.Num(); i++)
			{
				//Delete all the components
				SplineComponentArray[i]->DestroyComponent();
			}
			//Empty the array
			SplineComponentArray.Empty();

			//Update the arm length to set a length
			CameraBoom->TargetArmLength = 200;
			
			//Increment the throw power for each second while holding it down
			ThrowPowerScale += (DeltaTime * MaxThrowPower);
			
			//Get rotation of the camera so it is behind the player
			FRotator rot = FRotator(0, GetControlRotation().Yaw, 0);
			//Update the rotation of the actor only if it is the server
			if (HasAuthority())
			{
				SetActorRotation(rot, ETeleportType::ResetPhysics);
			}

			//Create Parameters for the
			FPredictProjectilePathParams params;
			params.StartLocation = GetMesh()->GetSocketLocation("Base-HumanPalmBone001Bone0015");

			//Starting Velocity of the throwing
			FVector velocity = GetControlRotation().Vector() + FVector(0, 0, 0.5f);
			velocity.Normalize();

			cacheVelocity = velocity * ThrowPowerScale * 10;
			params.LaunchVelocity = cacheVelocity;
			params.ProjectileRadius = 10;
			params.bTraceWithChannel = false;
			params.SimFrequency = 5;
			TArray<AActor*> actors;
			actors.Add(this);
			actors.Add(PickedUpItem);
			params.bTraceComplex = true;
			params.ActorsToIgnore = actors;

			FPredictProjectilePathResult result;
			UGameplayStatics::PredictProjectilePath(GetWorld(), params, result);

			for (int i = 0; i < result.PathData.Num(); i++)
			{
				SplineComponent->AddSplinePointAtIndex(result.PathData[i].Location, i, ESplineCoordinateSpace::Local, true);
			}

			for (int i = 0; i < (SplineComponent->GetNumberOfSplinePoints() - 1); i++)
			{
				USplineMeshComponent* spline = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
				if (SplineMesh)
				{
						spline->SetStaticMesh(SplineMesh);
				}
				spline->SetOnlyOwnerSee(true);
				spline->SetMaterial(0,SplineMeshMaterial);
				spline->SetStartScale(FVector2D(0.1f, 0.1f),true);
				spline->SetEndScale(FVector2D(0.1f, 0.1f), true);
				spline->SetForwardAxis(ESplineMeshAxis::Z);
				spline->RegisterComponentWithWorld(GetWorld());

				spline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				spline->SetMobility(EComponentMobility::Movable);

				const FVector startPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
				const FVector startTangent = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
				const FVector endPoint = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
				const FVector endTangent = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
				
				spline->SetStartAndEnd(startPoint, startTangent, endPoint, endTangent, true);
				spline->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				SplineComponentArray.Add(spline);
			}
		}
		else
		{
			Throw();  
		}
	}
	else
	{
		CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, cacheArmLength, DeltaTime);

		SplineComponent->ClearSplinePoints(true);
		for (int Index = 0; Index != SplineComponentArray.Num(); ++Index)
		{
			SplineComponentArray[Index]->DestroyComponent();
		}
		SplineComponentArray.Empty();
	}
	
	if (IsInteracting)
	{
		CanMove = false;
	}
	else
	{
		CanMove = true;
	}

	if (CheckHeavyItem())
	{
		GetCharacterMovement()->MaxWalkSpeed = 250.0f; 
	}
	else
	{
		//Updates climbing movement
		CheckClimbing();
	}


	IsPickingUp = false;
	CheckDeath(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &APlayerCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlayerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APlayerCharacter::TouchStopped);

	//Interacting/Hold Interacting of object
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::StopInteract);

	//Picking up and Dropping Items
	PlayerInputComponent->BindAction("Pickup / Throw", IE_Pressed, this, &APlayerCharacter::PickupAndDrop);

	//Throw picked up item
	PlayerInputComponent->BindAction("Pickup / Throw", IE_Released, this, &APlayerCharacter::Throw);

	//Crouching / Sneakinbg
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::StopCrouching);
}

void APlayerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (!CanMove)
	{
		return;
	}
	Jump();

	CanMove = true;
	IsInteracting = false;
}

void APlayerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (CameraBoom->bDoCollisionTest)
	{
		//Normal Camera
		AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		//Maze Camera 
		//Dont allow camera to go below the ground
		if (GetControlRotation().Pitch > 270 && GetControlRotation().Pitch <= 360)
		{
			AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
		}
		else
		{

			FRotator newRot = GetControlRotation();
			newRot.Pitch = 360;
			Controller->SetControlRotation(newRot);
		}
	}


}

void APlayerCharacter::MoveForward(float Value)
{
	if (!CanMove)
	{
		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward


		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (IsClimbing)
		{
			Direction = FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::X);
		}

		//Walking VS Crouching
		if (IsCrouching)
		{
			AddMovementInput(Direction, Value * CrouchSpeedScale);
		}
		else
		{
			AddMovementInput(Direction, Value);
		}

	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (!CanMove || IsClimbing)
	{
		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction

		if (IsCrouching)
		{
			AddMovementInput(Direction, Value * CrouchSpeedScale);
		}
		else
		{
			AddMovementInput(Direction, Value);
		}
	}
}

void APlayerCharacter::Interact()
{
	RPCInteract();
	if (CanInteract)
	{
		IsInteracting = true;
	}
}
void APlayerCharacter::StopInteract()
{
	RPCStopInteract();
}

void APlayerCharacter::RPCInteract_Implementation()
{
	if (CanInteract)
	{
		IsInteracting = true;
	}
}

void APlayerCharacter::RPCStopInteract_Implementation()
{
	IsInteracting = false; 
}

void APlayerCharacter::PickupAndDrop()
{
	ServerRPCPickupAndDrop();

	if (PickedUpItem)
	{
		APickupableKey* key = Cast<APickupableKey>(PickedUpItem);

		if (key)
		{
			IsPickingUp = true;
		}
		else
		{
			IsPickingUp = true;
		}
	}
}

void APlayerCharacter::ServerRPCPickupAndDrop_Implementation()
{
	ClientShowThrowWidget();

	if (PickedUpItem)
	{
		IsPickingUp = false;
		IsHoldingDownThrow = true;
		return;
	}

	TArray<FHitResult> OutHits;

	FVector SweepStart = GetActorLocation();

	FVector SweepEnd = GetActorLocation();

	//Create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(200.0f);

	//Check if something is hit
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, MyColSphere);

	if (isHit)
	{
		//Loop through TArray
		for (auto& Hit : OutHits)
		{
			APickupable* pickup = Cast<APickupable>(Hit.GetActor());

			if (pickup)
			{
				//If item is already picked up go to next actor
				if (pickup->Player)
				{
					continue;
				}

				ClientRPCPickupAndDrop(pickup);

				//Stop after picking up an item
				break;
			}

		}
	}
}

void APlayerCharacter::ClientRPCPickupAndDrop_Implementation(APickupable* _Pickup)
{
	//If the pickuped item is not null
	if (!_Pickup)
	{
		return;
	}
	_Pickup->Player = this;
	_Pickup->Mesh->SetSimulatePhysics(false);
	_Pickup->Mesh->SetCollisionProfileName("Trigger");

	_Pickup->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetMesh()->GetSocketBoneName("Base-HumanPalmBone001Bone0015"));

	PickedUpItem = _Pickup;

	IsPickingUp = true;

	_Pickup->PlayPickupSound();

}

void APlayerCharacter::ClientShowThrowWidget_Implementation()
{
	if (PickedUpItem)
	{
		IsHoldingDownThrow = true;

		CurrentThrowWidget = CreateWidget<UUserWidget>(GetWorld(), ThrowWidgetClass);

		if (ThrowWidgetClass != nullptr)
		{


			if (CurrentThrowWidget)
			{
				CurrentThrowWidget->AddToPlayerScreen();
			}
		}
	}
}


void APlayerCharacter::Throw()
{
	ServerRPCThrow();
	IsHoldingHeavyItem = false;
}

void APlayerCharacter::CheckDeath(float _DeltaTime)
{
	if (IsDead)
	{
		DeathTimerCounter += _DeltaTime;

		if (DeathTimerCounter > DeathTimer)
		{
			if (HasAuthority())
			{
				SetActorLocation(RespawnCheckpoint);
			}
			else
			{
				SetActorLocation(RespawnCheckpoint + FVector(5,5,5));
			}

			CanMove = true;

			DeathTimerCounter = 0.0f;
			HitByWallCount = 0;
			IsDead = false;
		}
	}
}

void APlayerCharacter::CheckClimbing_Implementation()
{
	if (IsClimbing)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = ClimbSpeed;
		GetCharacterMovement()->MaxStepHeight = 100.0f;
		GetCharacterMovement()->SetWalkableFloorAngle(90.0f);
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		GetCharacterMovement()->MaxStepHeight = 45.0f;
		GetCharacterMovement()->SetWalkableFloorAngle(60.0f);
	}
}



bool APlayerCharacter::CheckHeavyItem()
{
	if (IsHoldingHeavyItem)
	{
		return true;
	}

	return false;
}

void APlayerCharacter::StartCrouching_Implementation()
{
	IsCrouching = true;
	Crouch(true);

}

void APlayerCharacter::StopCrouching_Implementation()
{
	IsCrouching = false;
	UnCrouch(true);
}



void APlayerCharacter::ServerRPCThrow_Implementation()
{
	ClientRPCThrow();
}

void APlayerCharacter::ClientRPCThrow_Implementation()
{
	//Set picking up to false
	IsPickingUp = false;
	//If the player has an item and is holding down throw
	if (PickedUpItem && IsHoldingDownThrow)
	{
		//Release Throw
		IsHoldingDownThrow = false;

		//Deatch the pickupable and reset its variables
		PickedUpItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PickedUpItem->Mesh->SetCollisionProfileName("IgnoreOnlyPawn");
		PickedUpItem->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickedUpItem->Launch(cacheVelocity);
		PickedUpItem->Player = nullptr;

		if (CurrentThrowWidget)
		{
			CurrentThrowWidget->RemoveFromParent();
		}
		ThrowPowerScale = 0;

		PickedUpItem = nullptr;
		return;
	}
}

void APlayerCharacter::ServerCheckPickup_Implementation()
{
	CheckPickup();

}

void APlayerCharacter::CheckPickup_Implementation()
{
	if (PickedUpItem)
	{
		IsPickingUp = false;
		if (CurrentPickupWidget)
		{
			if (CurrentPickupWidget->IsVisible())
			{
				CurrentPickupWidget->RemoveFromParent();
			}
		}
		return;
	}

	TArray<FHitResult> OutHits;

	FVector SweepStart = GetActorLocation();

	FVector SweepEnd = GetActorLocation();

	//Create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(200.0f);

	//Check if something is hit
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, MyColSphere);

	if (isHit)
	{
		//Loop through TArray
		for (auto& Hit : OutHits)
		{
			APickupable* pickup = Cast<APickupable>(Hit.GetActor());

			if (pickup)
			{
				if (!pickup->IsInAir && !pickup->Player)
				{
					if (PickupWidgetClass != nullptr)
					{
						if (CurrentPickupWidget)
						{
							if (!CurrentPickupWidget->IsVisible())
							{
								CurrentPickupWidget->AddToPlayerScreen();
							}
						}
						break;
					}
				}
			}
			else
			{
				if (CurrentPickupWidget)
				{
					if (CurrentPickupWidget->IsVisible())
					{
						CurrentPickupWidget->RemoveFromParent();
					}
				}
			}

		}
	}
	IsPickingUp = false;
}


void APlayerCharacter::ServerCheckInteract_Implementation()
{
	CheckInteract();
}

void APlayerCharacter::CheckInteract_Implementation()
{
	if (CanInteract)
	{
		if (InteractWidgetClass != nullptr)
		{
			if (CurrentInteractWidget)
			{
				if (!CurrentInteractWidget->IsVisible())
				{
					CurrentInteractWidget->AddToPlayerScreen();
				}
			}
		}
	}
	else
	{
		if (CurrentInteractWidget)
		{
			if (CurrentInteractWidget->IsVisible())
			{
				CurrentInteractWidget->RemoveFromParent();
			}
		}
	}

}