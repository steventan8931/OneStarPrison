// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, OpenPosition);
	DOREPLIFETIME(ADoor, ClosedPosition);
	DOREPLIFETIME(ADoor, IsOpen);

	DOREPLIFETIME(ADoor, OpenRotation);
	DOREPLIFETIME(ADoor, ClosedRotation);
	DOREPLIFETIME(ADoor, IsRotationDoor);

}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
}

void ADoor::OpenDoor(float _DeltaTime)
{
	if (IsRotationDoor)
	{
		if (Mesh->GetComponentRotation() != OpenRotation)
		{
			FRotator newRot = FMath::Lerp(Mesh->GetComponentRotation(), OpenRotation, _DeltaTime);
			Mesh->SetWorldRotation(newRot);
		}
	}
	else
	{
		if (Mesh->GetComponentLocation() != OpenPosition)
		{
			FVector newPos = FMath::Lerp(Mesh->GetComponentLocation(), OpenPosition, _DeltaTime);
			Mesh->SetWorldLocation(newPos);
		}
	}

}

void ADoor::CloseDoor(float _DeltaTime)
{
	if (IsRotationDoor)
	{
		if (Mesh->GetComponentRotation() != ClosedRotation)
		{
			FRotator newRot = FMath::Lerp(Mesh->GetComponentRotation(), ClosedRotation, _DeltaTime);
			Mesh->SetWorldRotation(newRot);
		}
	}
	else
	{
		if (Mesh->GetComponentLocation() != ClosedPosition)
		{
			FVector newPos = FMath::Lerp(Mesh->GetComponentLocation(), ClosedPosition, _DeltaTime);
			Mesh->SetWorldLocation(newPos);
		}
	}

}

void ADoor::PlaySound()
{
	if (OpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
	}
}