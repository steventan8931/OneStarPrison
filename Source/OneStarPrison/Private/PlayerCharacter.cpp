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

	//Defaults
	PickedUpItem = nullptr;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority())
	{
		PlayerIndex = 1;
	}
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, IsInteracting);
	DOREPLIFETIME(APlayerCharacter, CanInteract);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsHoldingDownThrow)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, FString::Printf(TEXT("Throw power = %f"), ThrowPowerScale));
		if (ThrowPowerScale < MaxThrowPower)
		{
			ThrowPowerScale += (DeltaTime * MaxThrowPower);
		}
		else
		{
			Throw();              
		}
	}

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
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::Interact()
{
	//If the player is near an interactable
	//OnRep_IsInteracting();
	RPCInteract();
}
void APlayerCharacter::StopInteract()
{
	RPCStopInteract();
}

void APlayerCharacter::RPCInteract_Implementation()
{
	if (CanInteract)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("IM CLICKING INTERACTING"));
		IsInteracting = true;
	}
}

void APlayerCharacter::RPCStopInteract_Implementation()
{
	IsInteracting = false;
}

void APlayerCharacter::PickupAndDrop()
{
	RPCPickupAndDrop();
}

void APlayerCharacter::RPCPickupAndDrop_Implementation()
{
	if (PickedUpItem)
	{
		IsHoldingDownThrow = true;

		if (HUDWidgetClass != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("WIDGET CLASS EXIST"));

			CurrentWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex), HUDWidgetClass);

			if (CurrentWidget)
			{
				CurrentWidget->AddToPlayerScreen();
			}
		}

		return;
	}

	TArray<FHitResult> OutHits;

	FVector SweepStart = GetMesh()->GetSocketLocation("hand_r");

	FVector SweepEnd = GetMesh()->GetSocketLocation("hand_r");

	//Create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(100.0f);

	//Draw debug sphere
	DrawDebugSphere(GetWorld(), SweepStart, MyColSphere.GetSphereRadius(), 100, FColor::White, false, 1);

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

				pickup->Player = this;
				pickup->Mesh->SetSimulatePhysics(false);
				pickup->Mesh->SetCollisionProfileName("Trigger");
				
				pickup->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetMesh()->GetSocketBoneName("hand_r"));
				PickedUpItem = pickup;

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, *Hit.GetActor()->GetName());
				}

				//Stop after picking up an item
				break;
			}

		}
	}
}

void APlayerCharacter::Throw()
{
	RPCThrow();
}

void APlayerCharacter::RPCThrow_Implementation()
{
	if (PickedUpItem && IsHoldingDownThrow)
	{
		IsHoldingDownThrow = false;
		PickedUpItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PickedUpItem->Mesh->SetCollisionProfileName("BlockAllDynamic");
		PickedUpItem->Mesh->SetSimulatePhysics(true);
		FVector throwPower = GetActorForwardVector() * ThrowPowerScale;
		//PickedUpItem->Mesh->AddForce(throwPower);
		PickedUpItem->Mesh->AddForceAtLocation(throwPower * 150 * PickedUpItem->Mesh->GetMass(), GetMesh()->GetSocketLocation("hand_r"));
		PickedUpItem->Player = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, FString::Printf(TEXT("Throw power = %s"), *PickedUpItem->Mesh->GetComponentVelocity().ToString()));

		if (CurrentWidget)
		{
			CurrentWidget->RemoveFromParent();
		}
		ThrowPowerScale = 0;

		PickedUpItem = nullptr;
		return;
	}
}
