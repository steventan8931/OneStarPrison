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

	RPCCheckForPickUp();

}


void APickupPile::RPCCheckForPickUp_Implementation()
{
	CheckForPickUp();
}

AActor* APickupPile::SpawnActor()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("SPAWNED"));

	return(GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorTransform()));
}

void APickupPile::CheckForPickUp_Implementation()
{
	for (int Index = 0; Index != ListOfPickups.Num(); ++Index)
	{
		APickupable* pickup = Cast<APickupable>(ListOfPickups[Index]);
		if (pickup)
		{
			if (pickup->Player || pickup->IsInAir)
			{
				ListOfPickups[Index] = SpawnActor();
				break;
			}

		}
	}
}

void APickupPile::ServerSpawn_Implementation()
{
	ListOfPickups.SetNum(PickupsToSpawn);
	for (int Index = 0; Index != ListOfPickups.Num(); ++Index)
	{
		ListOfPickups[Index] = SpawnActor();
	}
}