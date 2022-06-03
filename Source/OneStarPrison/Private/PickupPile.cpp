// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupPile.h"
#include "Components/BoxComponent.h"
#include "Pickupable.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
APickupPile::APickupPile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("Trigger"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupPile::BeginPlay()
{
	Super::BeginPlay();
	
	ListOfPickups.SetNum(PickupsToSpawn);
	for (int Index = 0; Index != ListOfPickups.Num(); ++Index)
	{
		ListOfPickups[Index] = SpawnActor();
	}
}

// Called every frame
void APickupPile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForPickUp();
}

AActor* APickupPile::SpawnActor()
{
	return(GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorTransform()));
}

void APickupPile::CheckForPickUp()
{
	for (int Index = 0; Index != ListOfPickups.Num(); ++Index)
	{
		APickupable* pickup = Cast<APickupable>(ListOfPickups[Index]);
		if (pickup)
		{
			if (pickup->Player)
			{
				ListOfPickups[Index] = SpawnActor();
			}

		}
	}
}
