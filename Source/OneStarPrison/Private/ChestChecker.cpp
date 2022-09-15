// Fill out your copyright notice in the Description page of Project Settings.


#include "ChestChecker.h"
#include "PickupableChest.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

#include "Door.h"

// Sets default values
AChestChecker::AChestChecker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	ChestPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Book1Position"));
	ChestPosition->SetupAttachment(Mesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AChestChecker::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AChestChecker::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AChestChecker::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChestChecker::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChestChecker, Door);

	DOREPLIFETIME(AChestChecker, ChestInserted);

	DOREPLIFETIME(AChestChecker, CompleteSound);
	DOREPLIFETIME(AChestChecker, OverlappingPlayer);

}

// Called every frame
void AChestChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If all books are inserted
	if (ChestInserted)
	{
		//Don't do the code after
		return;
	}

	//If there is an overlapping player
	if (OverlappingPlayer)
	{
		//If they have an item
		if (OverlappingPlayer->PickedUpItem)
		{
			APickupableChest* chest = Cast<APickupableChest>(OverlappingPlayer->PickedUpItem);

			//If the held item is a book
			if (chest)
			{
				//Allow the player to interact
				OverlappingPlayer->CanInteract = true;

				//If the player clicked interact
				if (OverlappingPlayer->IsInteracting)
				{
					//Attach the book to this actor
					chest->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);


					ChestInserted = true;
					chest->SetActorLocation(ChestPosition->GetComponentLocation());

					//Play the insert sound
					if (InsertSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), InsertSound, GetActorLocation());
					}

					if (Door)
					{
						Door->IsOpen = true;
						Door->PlaySound();
					}

					//Make the player unable to continue interacting
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->IsInteracting = false;
					//Remove the item from the player
					OverlappingPlayer->PickedUpItem = nullptr;
					
					chest->Player = nullptr;
					chest->OnDisplay = false;

					OverlappingPlayer->IsHoldingHeavyItem = false;

				}

			}
		}
	}
}



void AChestChecker::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If there isn't already an overlapping player
			if (!OverlappingPlayer)
			{
				//Set the current player to the overlapping player
				OverlappingPlayer = playerActor;
			}
		}

	}
}

void AChestChecker::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If an overlapping player exists
			if (OverlappingPlayer)
			{
				//If the current player is the overlapping player
				if (playerActor == OverlappingPlayer)
				{
					//Make the overlapping player can't interact 
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer = nullptr;
				}
			}
		}
	}
}