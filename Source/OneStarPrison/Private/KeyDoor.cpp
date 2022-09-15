// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyDoor.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "PickupableKey.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AKeyDoor::AKeyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	RootComponent = BoxCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKeyDoor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AKeyDoor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AKeyDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AKeyDoor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKeyDoor, OpenPosition);
	DOREPLIFETIME(AKeyDoor, IsKeyOneTimeUse);
	DOREPLIFETIME(AKeyDoor, IsOpen);
}

// Called every frame
void AKeyDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Update the door to its open position when open
	if (IsOpen)
	{
		OpenDoor(DeltaTime);
		return;
	}

	//If there is an overlapping player
	if (OverlappingPlayer)
	{
		APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer->PickedUpItem);
		//If the overlapping player has a key
		if (key)
		{
			//If the key matches allow them to interact
			if (key->KeyCode == KeyCode)
			{
				OverlappingPlayer->CanInteract = true;
				//Change the players interact type to lever pulling
				OverlappingPlayer->InteractType = EInteractType::LeverPull;
			}

			//If they are interacting
			if (OverlappingPlayer->IsInteracting)
			{
				//Remove the key if it is only a one time use
				if (IsKeyOneTimeUse)
				{
					OverlappingPlayer->PickedUpItem->Destroy();
					OverlappingPlayer->PickedUpItem = nullptr;
				}

				//Play the door open sound
				if (OpenSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
				}

				//Open the door
				IsOpen = true;
				//Remove the players ability to further interact with this door
				OverlappingPlayer->CanInteract = false;			
			}
		}

	}
}

void AKeyDoor::OpenDoor(float _DeltaTime)
{
	//Updates the door to its open position over time
	if (Mesh->GetComponentLocation() != OpenPosition)
	{
		FVector newPos = FMath::Lerp(Mesh->GetComponentLocation(), OpenPosition, _DeltaTime);
		Mesh->SetWorldLocation(newPos);
	}
}

void AKeyDoor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If overlapping player is null, set the overlapping player to the current player
			if (OverlappingPlayer == nullptr)
			{
				OverlappingPlayer = playerActor;
			}
			else
			{
				//If the overlapping actor isn't null and the overlapping actor has a key
				APickupableKey* key = Cast<APickupableKey>(playerActor->PickedUpItem);
				if (key)
				{
					//If the key matches
					if (key->KeyCode == KeyCode)
					{
						//Make the overlapping actor the current player and allow them to interact
						playerActor->CanInteract = true;
						OverlappingPlayer = playerActor;
						//Change the players interact type to lever pulling
						OverlappingPlayer->InteractType = EInteractType::LeverPull;
					}
				}
			}
		}
	
	}
}

void AKeyDoor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If the overlapping player exists
			if (OverlappingPlayer)
			{
				//If this overlapping player is the overlapping player
				if (playerActor == OverlappingPlayer)
				{
					//Remove the players ability to interact with the lever
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer = nullptr;
				}
			}
		}
	}
}

