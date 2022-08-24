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
	//Spawns at rock at the start to be picked up
	ServerSpawn();
}

void APickupPile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupPile, ListOfPickups);
}

// Called every frame
void APickupPile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Have the server check for pickups
	RPCCheckForPickUp();
}


void APickupPile::RPCCheckForPickUp_Implementation()
{
	//Check for pick up on the clients
	CheckForPickUp();
}

//Spawns the actor at this actors transform
AActor* APickupPile::SpawnActor()
{
	return(GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorTransform()));
}

void APickupPile::CheckForPickUp_Implementation()
{
	//Iterate through the spawn items
	for (int i = 0; i < ListOfPickups.Num(); i++)
	{
		APickupable* pickup = Cast<APickupable>(ListOfPickups[i]);

		//If the spawn actors are valid
		if (pickup)
		{
			//If the spawned actors have been picked up by a player or have been thrown
			if (pickup->Player || pickup->IsInAir)
			{
				//Spawn another one
				ListOfPickups[i] = SpawnActor();
				break;
			}

		}
	}
}

void APickupPile::ServerSpawn_Implementation()
{
	//Set the number of
	ListOfPickups.SetNum(1);
	ListOfPickups[0] = SpawnActor();
}