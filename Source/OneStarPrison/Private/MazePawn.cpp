// Fill out your copyright notice in the Description page of Project Settings.


#include "MazePawn.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMazePawn::AMazePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
}

// Called when the game starts or when spawned
void AMazePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMazePawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMazePawn, EndCameraTransform);
	DOREPLIFETIME(AMazePawn, CurrentCameraTransform);
}

// Called every frame
void AMazePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMazePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Interacting/Hold Interacting of object
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMazePawn::ExitPawn);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMazePawn::MoveRight);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMazePawn::MoveForward);
}


void AMazePawn::ExitPawn()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (CurrentPlayer)
	{
		controller->Possess(CurrentPlayer);
	}

}

void AMazePawn::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//AddMovementInput(Direction, Value);
		Cam->AddWorldOffset(Direction * Value);
	}
}

void AMazePawn::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction

		//AddMovementInput(Direction, Value);	
		Cam->AddWorldOffset(Direction * Value);
	}
}