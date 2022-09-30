// Copyright Epic Games, Inc. All Rights Reserved.

#include "AI/Cpp_AICharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "AI/ai_tags.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "AI/NPC.h"

//////////////////////////////////////////////////////////////////////////
// ACpp_AICharacter

ACpp_AICharacter::ACpp_AICharacter()
{
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
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACpp_AICharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACpp_AICharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACpp_AICharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACpp_AICharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACpp_AICharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACpp_AICharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACpp_AICharacter::TouchStopped);
}

void ACpp_AICharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACpp_AICharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ACpp_AICharacter::OnExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void ACpp_AICharacter::SetupStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

void ACpp_AICharacter::OnAttack()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
}

void ACpp_AICharacter::OnDistract()
{
	if (DistractionSound)
	{
		FVector const Loc = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DistractionSound, Loc);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), Loc, 1.0f, this, 0.0f, tags::NoiseTag);
	}
}

void ACpp_AICharacter::OnAttackOverlapBegin(
	UPrimitiveComponent* const OverlappedComponent,
	AActor* const OtherActor,
	UPrimitiveComponent* OtherComponent,
	int const OtherBodyIndex,
	bool const FromSweep,
	FHitResult const& SweepResult)
{
	if (ANPC* const NPC = Cast<ANPC>(OtherActor))
	{
		float const NewHealth = NPC->GetHealth() - NPC->GetMaxHealth() * 0.1f;
		NPC->SetHealth(NewHealth);
	}
}

void ACpp_AICharacter::OnAttackOverlapEnd(
	UPrimitiveComponent* const OverlappedComponent,
	AActor* const OtherActor,
	UPrimitiveComponent* OtherComponent,
	int const OtherBodyIndex)
{
}

void ACpp_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	// limit pitch of camera
	APlayerCameraManager* const CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	CamManager->ViewPitchMin = -50.0f;
	CamManager->ViewPitchMax = 10.0f;

	// set material color of character
	UMaterialInstanceDynamic* const MaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue("BodyColor", FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
		GetMesh()->SetMaterial(0, MaterialInstance);
	}

	// attach delegates to the collision box
	if (RightFistCollisionBox)
	{
		RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACpp_AICharacter::OnAttackOverlapBegin);
		RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACpp_AICharacter::OnAttackOverlapEnd);
	}
}

float ACpp_AICharacter::GetHealth() const
{
	return Health;
}

float ACpp_AICharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void ACpp_AICharacter::SetHealth(float const NewHealth)
{
	Health = NewHealth;
	if (Health <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("You lose sucker!!!"));
		auto const Cont = UGameplayStatics::GetPlayerController(this, 0);
		Cont->SetCinematicMode(true, false, false, true, true);
		GetMesh()->SetSimulatePhysics(true);
		GetMovementComponent()->MovementState.bCanJump = false;
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
}

void ACpp_AICharacter::Tick(float const DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACpp_AICharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACpp_AICharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACpp_AICharacter::MoveForward(float Value)
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

void ACpp_AICharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
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
