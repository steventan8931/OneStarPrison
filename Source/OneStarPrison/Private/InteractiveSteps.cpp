// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveSteps.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AInteractiveSteps::AInteractiveSteps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AInteractiveSteps::BeginPlay()
{
	Super::BeginPlay();
	ClosedPosition = GetActorLocation();
}

void AInteractiveSteps::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveSteps, OpenPosition);
	DOREPLIFETIME(AInteractiveSteps, ClosedPosition);
	DOREPLIFETIME(AInteractiveSteps, IsOpen);

	DOREPLIFETIME(AInteractiveSteps, CloseTimer);
	DOREPLIFETIME(AInteractiveSteps, CloseDelay);
}

// Called every frame
void AInteractiveSteps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Update the door position based on whether it is open or closed
	if (IsOpen)
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
}

void AInteractiveSteps::OpenDoor(float _DeltaTime)
{
	//Resets the close timer when it is open
	CloseTimer = 0.0f;

	//Update the position of the step to its open position over time
	if (GetActorLocation() != OpenPosition)
	{
		FVector newPos = FMath::Lerp(GetActorLocation(), OpenPosition, _DeltaTime);

		SetActorLocation(newPos);
	}
}

void AInteractiveSteps::CloseDoor(float _DeltaTime)
{
	//Increment the close timer over time
	CloseTimer += _DeltaTime;

	//If the close timer has reached the close delay time
	if (CloseTimer >= CloseDelay)
	{
		//Update the position of the step to its closed position over time
		if (GetActorLocation() != ClosedPosition)
		{
			FVector newPos = FMath::Lerp(GetActorLocation(), ClosedPosition, _DeltaTime);
			SetActorLocation(newPos);
		}
	}

}
