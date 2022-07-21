// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableDisplay.h"
#include "Components/ChildActorComponent.h"

//Particles
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Pickupable.h"

// Sets default values
APickupableDisplay::APickupableDisplay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	PickupableToDisplay = CreateDefaultSubobject<UChildActorComponent>(TEXT("PickupableChild"));
	PickupableToDisplay->SetupAttachment(RootComponent);

	Particles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particles"));
	Particles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupableDisplay::BeginPlay()
{
	Super::BeginPlay();

	if (PickupableToDisplay)
	{
		StartingHeight = PickupableToDisplay->GetRelativeLocation().Z;
	}

}

// Called every frame
void APickupableDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PickupableToDisplay)
	{
		APickupable* pickupable = Cast<APickupable>(PickupableToDisplay->GetChildActor());

		if (pickupable)
		{
			if (!pickupable->Player)
			{
				float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));

				FVector newLoc = PickupableToDisplay->GetRelativeLocation();
				newLoc.Z += DeltaHeight * FloatScale;

				RunningTime += DeltaTime;

				FRotator newRot = PickupableToDisplay->GetRelativeRotation();
				newRot.Yaw += RotateScale;

				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("moving")));
				
				pickupable->Mesh->SetRelativeLocation(newLoc);
				pickupable->Mesh->SetRelativeRotation(newRot);

				//PickupableToDisplay->SetRelativeLocation(newLoc);
				//PickupableToDisplay->SetRelativeRotation(newRot);
			}


		}

	}

}

