// Fill out your copyright notice in the Description page of Project Settings.


#include "CrankliftPlatform.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

#include "PickupableChest.h"

// Sets default values
ACrankliftPlatform::ACrankliftPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 100));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACrankliftPlatform::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ACrankliftPlatform::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ACrankliftPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrankliftPlatform::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACrankliftPlatform, IsWeightDependent);
}

// Called every frame
void ACrankliftPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If there is a player or a weight
	if (OverlappingPlayer && OverlappingWeight)
	{
		//If the player is holding an item
		if (OverlappingPlayer->PickedUpItem)
		{
			APickupableChest* chest = Cast<APickupableChest>(OverlappingPlayer->PickedUpItem);

			//If the item is a chest
			if (chest)
			{
				OverlappingWeight = chest;
				//Dont allow the platform to be moved
				CanMove = false;
				return;
			}
		}

		CanMove = false;
		return;
	}

	//If the player has a item but it isn't the chest or doesn't have the item allow it to move
	CanMove = true;
}

void ACrankliftPlatform::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If the platform isn't weight dependent ignore 
	if (!IsWeightDependent)
	{
		return;
	}

	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			//If the overlapping actor is a player
			if (playerActor)
			{
				//Set the overlapping player to this player
				OverlappingPlayer = playerActor;

				//If the player is holding an item
				if (playerActor->PickedUpItem)
				{
					APickupableChest* chest = Cast<APickupableChest>(playerActor->PickedUpItem);

					//If the item is a chest
					if (chest)
					{
						OverlappingWeight = chest;
						//Dont allow the platform to be moved
						CanMove = false;
						return;
					}
				}
			}


			APickupableChest* chest = Cast<APickupableChest>(OtherActor);
			
			//If the overlapping actor is a chest
			if (chest)
			{
				//Set the overlapping weight to the chest
				OverlappingWeight = chest; 
				return;
			}
		}

		//If the player has a item but it isn't the chest or doesn't have the item allow it to move
		CanMove = true; 
	}
}

void ACrankliftPlatform::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//If the platform isn't weight dependent ignore 
	if (!IsWeightDependent)
	{
		return;
	}

	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping player exists
		if (OverlappingPlayer)
		{
			//If this overlapping player is the overlapping player
			if (playerActor == OverlappingPlayer)
			{
				//Set Overlapping player to null
				OverlappingPlayer = nullptr;
			}
		}

		APickupableChest* chest = Cast<APickupableChest>(OtherActor);

		if (OverlappingWeight)
		{
			if (chest == OverlappingWeight)
			{
				OverlappingWeight = nullptr;
			}
		}

		//If only one of them are overlapping
		if (!OverlappingPlayer || !OverlappingWeight)
		{
			CanMove = true;
		}
	}
}