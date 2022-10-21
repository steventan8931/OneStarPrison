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

	//Crouching / Sneaking
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::StopCrouching);
}

void APlayerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
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
		if (GetControlRotation().Pitch > 270 && GetControlRotation().Pitch <= 360)
		{
			//Normal camera look up at rate
			AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
		}
		else
		{
			//Dont allow camera to go below the ground
			FRotator newRot = GetControlRotation();
			newRot.Pitch = 360;
			Controller->SetControlRotation(newRot);
		}
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	//If the player can't move don't update movement
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

		//If the player is climbing move the player is its actor facing direction instead of camera facing
		if (IsClimbing)
		{
			Direction = FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::X);
		}

		//Walking VS Crouching
		if (IsCrouching)
		{
			//Have the player move slower when they are crouching
			AddMovementInput(Direction, Value * CrouchSpeedScale);
		}
		else
		{
			//Use default movement if they aren't crouching
			AddMovementInput(Direction, Value);
		}

	}
}

void APlayerCharacter::MoveRight(float Value)
{
	//If the player can't move or is climbing don't update movement
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

		//Walking VS Crouching
		if (IsCrouching)
		{
			//Have the player move slower when they are crouching
			AddMovementInput(Direction, Value * CrouchSpeedScale);
		}
		else
		{
			//Use default movement if they aren't crouching
			AddMovementInput(Direction, Value);
		}
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Cache the starting point as the players respawn checkpoint
	RespawnCheckpoint = GetActorLocation();

	CurrentInteractWidget = CreateWidget<UUserWidget>(GetWorld(), InteractWidgetClass);
	CurrentPickupWidget = CreateWidget<UUserWidget>(GetWorld(), PickupWidgetClass);

	//cache the arm legnth as the starting arm length of the camera boom
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

	DOREPLIFETIME(APlayerCharacter, ThrowEndActorType);
	DOREPLIFETIME(APlayerCharacter, cacheThrowEndActor);
	
	DOREPLIFETIME(APlayerCharacter, CanPush);
	DOREPLIFETIME(APlayerCharacter, IsPushing);

	DOREPLIFETIME(APlayerCharacter, InteractType);
	DOREPLIFETIME(APlayerCharacter, IsHoldingHeavyItem);

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

	//Have the server
	ServerCheckPickup();
	ServerCheckInteract();

	ShowProjectilePath(DeltaTime);
	
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

//When the player presses the interact button
void APlayerCharacter::Interact_Implementation()
{
	//If the player presses the interact button and is able to interact
	if (CanInteract)
	{
		//Set Is Interacting to true
		IsInteracting = true;
	}

	ServerInteract();
}

//When the player presses the interact button
void APlayerCharacter::ServerInteract_Implementation()
{
	//If the player presses the interact button and is able to interact
	if (CanInteract)
	{
		//Set Is Interacting to true
		IsInteracting = true;
	}
}

//When the player releases the interact button
void APlayerCharacter::StopInteract_Implementation()
{
	//Set Is Interacting to false
	IsInteracting = false;

	ServerStopInteract();
}

//When the player presses the interact button
void APlayerCharacter::ServerStopInteract_Implementation()
{
	//Set Is Interacting to false
	IsInteracting = false;
}

//Have the server to check for the interact
void APlayerCharacter::ServerCheckInteract_Implementation()
{
	CheckInteract();
}

//Check whether to show interact prompt on the client
void APlayerCharacter::CheckInteract_Implementation()
{
	//If the player can interact
	if (CanInteract)
	{
		//If the interact widget class has been filled out
		if (InteractWidgetClass)
		{
			//If the widget exist
			if (CurrentInteractWidget)
			{
				//If it hasn't been rendered to the screen
				if (!CurrentInteractWidget->IsVisible())
				{
					//Render it to the player's screen
					CurrentInteractWidget->AddToPlayerScreen();
				}
			}
		}
	}
	else
	{
		//If the widget exist
		if (CurrentInteractWidget)
		{
			//If it is rendered to the screen
			if (CurrentInteractWidget->IsVisible())
			{
				//Remove it from the player's screen
				CurrentInteractWidget->RemoveFromParent();
			}
		}
	}

}

//When the player presses the pick up button
void APlayerCharacter::PickupAndDrop()
{
	ServerRPCPickupAndDrop();

	if (PickedUpItem)
	{
		IsPickingUp = true;		
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

	//Set up parameters for sweep multi by channel
	TArray<FHitResult> OutHits;
	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = GetActorLocation();

	//Create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(200.0f);

	//Check if something is hit
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, MyColSphere);

	//It something was hit
	if (isHit)
	{
		//Loop through TArray
		for (auto& Hit : OutHits)
		{
			APickupable* pickup = Cast<APickupable>(Hit.GetActor());

			//If the hit actor was a pickup
			if (pickup)
			{
				//If item has already been picked up or is in the air go to next actor 
				if (pickup->Player || pickup->IsInAir)
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

//Have the server to check for the pick up
void APlayerCharacter::ServerCheckPickup_Implementation()
{
	CheckPickup();
}

//Check whether to show pick up prompt on the client
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

	//Set up parameters for sweep multi by channel
	TArray<FHitResult> OutHits;
	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = GetActorLocation();

	//Create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(200.0f);

	//Check if something is hit
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, MyColSphere);

	//It something was hit
	if (isHit)
	{
		//Loop through TArray
		for (auto& Hit : OutHits)
		{
			APickupable* pickup = Cast<APickupable>(Hit.GetActor());

			//If the hit actor was a pickup
			if (pickup)
			{
				//If item hasn't picked up and isn't in the air
				if (!pickup->IsInAir && !pickup->Player)
				{
					//If the pick up widget class has been filled out
					if (PickupWidgetClass)
					{
						//If the widget exist
						if (CurrentPickupWidget)
						{
							//If it hasn't been rendered to the screen
							if (!CurrentPickupWidget->IsVisible())
							{
								//Render it to the player's screen
								CurrentPickupWidget->AddToPlayerScreen();
							}
						}
						break;
					}
				}
			}
			else
			{
				//If the widget exist
				if (CurrentPickupWidget)
				{
					//If it is rendered to the screen
					if (CurrentPickupWidget->IsVisible())
					{
						//Remove it from the player's screen
						CurrentPickupWidget->RemoveFromParent();
					}
				}
			}

		}
	}

	//Reset variables
	IsPickingUp = false;
}

void APlayerCharacter::ClientRPCPickupAndDrop_Implementation(APickupable* _Pickup)
{
	//If the pickuped item is not null
	if (!_Pickup)
	{
		return;
	}
	
	//Set the owning player of the pickup to this player
	_Pickup->Player = this;
	//Make the mesh of the pickup not simulate physics
	_Pickup->Mesh->SetSimulatePhysics(false);
	//Remove collision of the mesh
	_Pickup->Mesh->SetCollisionProfileName("Trigger");

	//Attach the picked up item to the player's hand socket
	_Pickup->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetMesh()->GetSocketBoneName("Base-HumanPalmBone001Bone0015"));

	//Store this item as a variable
	PickedUpItem = _Pickup;

	//Set IsPickingUp to true for playing of the pick up animation
	IsPickingUp = true;

	//Play the pick up sound of the picked up item
	_Pickup->PlayPickupSound();

}

void APlayerCharacter::ClientShowThrowWidget_Implementation()
{
	if (PickedUpItem)
	{
		IsHoldingDownThrow = true;

		//Create the throw UI
		CurrentThrowWidget = CreateWidget<UUserWidget>(GetWorld(), ThrowWidgetClass);

		//If the throw widget class has been filled out
		if (ThrowWidgetClass)
		{
			//If the widget exist
			if (CurrentThrowWidget)
			{
				//Render it to the player's screen
				CurrentThrowWidget->AddToPlayerScreen();
			}
		}
	}
}


void APlayerCharacter::Throw_Implementation()
{
	ClientRPCThrow();
	IsHoldingHeavyItem = false;
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
		//Make it ignore only the player
		PickedUpItem->Mesh->SetCollisionProfileName("IgnoreOnlyPawn");
		//Re-enable collision of query and physics
		PickedUpItem->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//Launch the picked up item at the cache velocity which was set in the predict projectile path
		PickedUpItem->Launch(cacheVelocity);
		//Remove the pointer reference from the item
		PickedUpItem->Player = nullptr;

		//If the throw widget exists
		if (CurrentThrowWidget)
		{
			//Remove it from the player's screen
			CurrentThrowWidget->RemoveFromParent();
		}

		//Reset Varibles
		ThrowPowerScale = 0;
		PickedUpItem = nullptr;
	}
}

void APlayerCharacter::ShowProjectilePath(float _DeltaTime)
{
	//Check if the player is holding down throw
	if (IsHoldingDownThrow)
	{
		FVector cameraOffset = FMath::Lerp(CameraBoom->GetRelativeLocation(), ThrowCamOffset, _DeltaTime * ThrowCamSpeed);
		CameraBoom->SetRelativeLocation(cameraOffset);

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
		if (ThrowPowerScale < MaxThrowPower)
		{
			//Increment the throw power for each second while holding it down
			ThrowPowerScale += (_DeltaTime * MaxThrowPower);
		}

		//Get rotation of the camera so it is behind the player
		// 
		ServerUpdateRotation();
		//FRotator rot = FRotator(0, GetControlRotation().Yaw, 0);
		//Update the rotation of the actor only if it is the server
		//if (HasAuthority())
		//{
			//SetActorRotation(rot, ETeleportType::ResetPhysics);
		//}

		//Create Parameters for the
		FPredictProjectilePathParams params;
		params.StartLocation = GetMesh()->GetSocketLocation("Base-HumanPalmBone001Bone0015");

		//Starting Velocity of the throwing
		FVector velocity = GetControlRotation().Vector() + FVector(0, 0, 0.5f);
		velocity.Normalize();

		if (PickedUpItem)
		{
			//cache this velocity for the throwing of the item
			cacheVelocity = (velocity * ThrowPowerScale * 10) / PickedUpItem->Mass;
		}


		//Set the parameters for throwing projectile
		params.LaunchVelocity = cacheVelocity;
		params.ProjectileRadius = 10;
		//params.bTraceWithChannel = true;
		params.SimFrequency = 5;
		//Make an array of actors for it to ignore
		TArray<AActor*> actors;
		//Make the projectile prediction ignore the player and the item being thrown
		actors.Add(this);
		actors.Add(PickedUpItem);
		//params.bTraceComplex = false;
		params.ActorsToIgnore = actors;

		//Set Collision channels to get hit result
		params.bTraceWithCollision = true;
		params.bTraceWithChannel = true;
		params.TraceChannel = ECollisionChannel::ECC_WorldDynamic;

		//Set up the predict projectile path
		FPredictProjectilePathResult result;
		UGameplayStatics::PredictProjectilePath(GetWorld(), params, result);

		ShowProjectileEnd(result.HitResult.ImpactPoint);

		//Iterate through the predicted path and set up spline points at each point of the path
		for (int i = 0; i < result.PathData.Num(); i++)
		{
			SplineComponent->AddSplinePointAtIndex(result.PathData[i].Location, i, ESplineCoordinateSpace::Local, true);
		}

		//Iterate through each spline point
		for (int i = 0; i < (SplineComponent->GetNumberOfSplinePoints() - 1); i++)
		{
			//Create a spline mesh at each point

			USplineMeshComponent* spline = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			if (SplineMesh)
			{
				spline->SetStaticMesh(SplineMesh);
			}
			//Make it so only the owning player can see the throwing path
			spline->SetOnlyOwnerSee(true);

			//Set parameters for each spline mesh point
			spline->SetMaterial(0, SplineMeshMaterial);
			spline->SetStartScale(FVector2D(0.1f, 0.1f), true);
			spline->SetEndScale(FVector2D(0.1f, 0.1f), true);
			spline->SetForwardAxis(ESplineMeshAxis::Z);
			spline->RegisterComponentWithWorld(GetWorld());

			spline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			spline->SetMobility(EComponentMobility::Movable);

			//Set the start and end points and tangent for each spline point
			const FVector startPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			const FVector startTangent = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			const FVector endPoint = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			const FVector endTangent = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

			spline->SetStartAndEnd(startPoint, startTangent, endPoint, endTangent, true);

			//Make the spline mesh not have collision (so it doesn't count for players overlapping
			spline->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//Add this spline mesh to the array to remove delete later
			SplineComponentArray.Add(spline);
		}
	}
	else
	{
		if (cacheThrowEndActor)
		{
			cacheThrowEndActor->SetActorHiddenInGame(true);
		}

		//Slowly reset the camera boom to its original length
		CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, cacheArmLength, _DeltaTime);

		FVector cameraOffset = FMath::Lerp(CameraBoom->GetRelativeLocation(), FVector(0.0f, 0.0f, 0.0f), _DeltaTime * ThrowCamSpeed);
		CameraBoom->SetRelativeLocation(cameraOffset);

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

	}
}

void APlayerCharacter::ShowProjectileEnd_Implementation(FVector _Location)
{
	if (ThrowEndActorType)
	{
		if (!cacheThrowEndActor)
		{
			FTransform transform = FTransform(FRotator(0, 0, 0), _Location, FVector(1, 1, 1));
			cacheThrowEndActor = GetWorld()->SpawnActor<AActor>(ThrowEndActorType, transform);
		}
		else
		{
			if (_Location != FVector(0, 0, 0))
			{
				cacheThrowEndActor->SetActorLocation(_Location);

				//cacheThrowEndActor->SetActorRotation(result.HitResult.ImpactNormal);
				cacheThrowEndActor->SetActorHiddenInGame(false);
			}
		}
	}
}

//Update Camera so it is behind the player
void APlayerCharacter::UpdateRotation_Implementation(FRotator _rot)
{
	//ServerUpdateRotation();
	SetActorRotation(_rot, ETeleportType::ResetPhysics);
}

//Update Camera so it is behind the player
void APlayerCharacter::ServerUpdateRotation_Implementation()
{
	FRotator rot = FRotator(0, GetControlRotation().Yaw, 0);

	UpdateRotation(rot);
}

//When the player presses the crouch button Makes the player crouch
void APlayerCharacter::StartCrouching_Implementation()
{
	//Sets crouching to true
	IsCrouching = true;
	//Use the character movement component's crouch function
	Crouch(true);

	ServerStartCrouching();
}

//When the player presses the crouch button Makes the player crouch on the server
void APlayerCharacter::ServerStartCrouching_Implementation()
{
	//Sets crouching to true
	IsCrouching = true;
	//Use the character movement component's crouch function
	Crouch(true);
}


//When the player releases the crouch button makes the player uncrouch
void APlayerCharacter::StopCrouching_Implementation()
{
	//Sets crouching to false
	IsCrouching = false;
	//Use the character movement component's crouch function
	UnCrouch(true);

	ServerStopCrouching();
}

//When the player releases the crouch button makes the player uncrouch
void APlayerCharacter::ServerStopCrouching_Implementation()
{
	//Sets crouching to false
	IsCrouching = false;
	//Use the character movement component's crouch function
	UnCrouch(true);
}

void APlayerCharacter::CheckDeath(float _DeltaTime)
{
	//If the player is dead
	if (IsDead)
	{
		//Increase the counter over time
		DeathTimerCounter += _DeltaTime;

		//Once the counter has reached the death timer
		if (DeathTimerCounter > DeathTimer)
		{
			//Repsawn the player
			if (HasAuthority())
			{
				//At the respawn checkpoint location
				SetActorLocation(RespawnCheckpoint);
			}
			else
			{
				//At an offset to the checkpoint location
				SetActorLocation(RespawnCheckpoint + FVector(5,5,5));
			}

			//Reset Variables
			CanMove = true;
			CanInteract = false;
			IsInteracting = false;
			IsHoldingHeavyItem = false;
			DeathTimerCounter = 0.0f;
			HitByWallCount = 0;
			IsDead = false;
		}
	}
}

void APlayerCharacter::CheckClimbing_Implementation()
{
	//Updates player character movement variables based on whether they are climbing or not
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


//Returns whether the player is holding an heavy item or not
bool APlayerCharacter::CheckHeavyItem()
{
	if (IsHoldingHeavyItem && PickedUpItem)
	{
		return true;
	}

	return false;
}


