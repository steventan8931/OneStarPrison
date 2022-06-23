// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawbridgePlatform.h"

// Sets default values
ADrawbridgePlatform::ADrawbridgePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ADrawbridgePlatform::BeginPlay()
{
	Super::BeginPlay();
	
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
	DOREPLIFETIME(APlayerCharacter, IsGrabbing);

}

// Called every frame
void ADrawbridgePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		OpenPlatform(DeltaTime);
	}
}

void ADrawbridgePlatform::OpenPlatform(float _DeltaTime)
{
	if (GetActorRotation() != OpenRotation)
	{
		FRotator newRot = FMath::Lerp(GetActorRotation(), OpenRotation, _DeltaTime * 2.0f);
		SetActorRotation(newRot);
	}
}