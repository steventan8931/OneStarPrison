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
			if (!pickupable->Player && !pickupable->TakenFromDisplay)
			{
				float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));

				FVector newLoc = pickupable->Mesh->GetRelativeLocation();
				newLoc.Z += DeltaHeight * FloatScale;

				RunningTime += DeltaTime;

				FRotator newRot = pickupable->Mesh->GetRelativeRotation();
				newRot.Yaw += RotateScale;
				
				pickupable->Mesh->SetRelativeLocation(newLoc);
				pickupable->Mesh->SetRelativeRotation(newRot);

				//PickupableToDisplay->SetRelativeLocation(newLoc);
				//PickupableToDisplay->SetRelativeRotation(newRot);

				Particles->SetVisibility(true);
			}
			else
			{
				Particles->SetVisibility(false);
			}

		}

	}

}

