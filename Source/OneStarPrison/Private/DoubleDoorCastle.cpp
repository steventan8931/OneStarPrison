// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleDoorCastle.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "PickupableKey.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoubleDoorCastle::ADoubleDoorCastle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	RootComponent = BoxCollision;

	LMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LMesh"));
	LMesh->SetupAttachment(RootComponent);

	RMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RMesh"));
	RMesh->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoubleDoorCastle::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ADoubleDoorCastle::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ADoubleDoorCastle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoubleDoorCastle::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoubleDoorCastle, LOpenRotation);
	DOREPLIFETIME(ADoubleDoorCastle, ROpenRotation);
	DOREPLIFETIME(ADoubleDoorCastle, NumOfOverlappingPlayers);

	DOREPLIFETIME(ADoubleDoorCastle, KeysRequired);
	DOREPLIFETIME(ADoubleDoorCastle, KeysInserted);

	DOREPLIFETIME(ADoubleDoorCastle, OverlappingPlayer);
	DOREPLIFETIME(ADoubleDoorCastle, OverlappingPlayer2);

	DOREPLIFETIME(ADoubleDoorCastle, IsOpen);

	DOREPLIFETIME(ADoubleDoorCastle, LClosedRotation);
	DOREPLIFETIME(ADoubleDoorCastle, RClosedRotation);

	DOREPLIFETIME(ADoubleDoorCastle, OpenSound);
	DOREPLIFETIME(ADoubleDoorCastle, InsertSound);
}


// Called every frame
void ADoubleDoorCastle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Opens/Closes the door based on "IsOpen"
	if (IsOpen)
	{
		OpenDoor(DeltaTime);
		return;
	}
	else
	{
		CloseDoor(DeltaTime);
	}

	//If there are keys required check whether they are inserted or not
	//and ignores the overlapping code
	if (KeysRequired > 0)
	{
		//Check players for the key interaction
		CheckKeyDoor(OverlappingPlayer);
		CheckKeyDoor(OverlappingPlayer2);
		return;
	}

	//If keys aren't required check that there are two players overlapping
	if (NumOfOverlappingPlayers == 2)
	{
		IsOpen = true;
		if (OpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
		}
	}

}

void ADoubleDoorCastle::OpenDoor(float _DeltaTime)
{
	//Update the left mesh to its open rotation over time
	if (LMesh->GetComponentRotation() != LOpenRotation)
	{
		FRotator newRotL = FMath::Lerp(LMesh->GetRelativeRotation(), LOpenRotation, _DeltaTime);
		LMesh->SetRelativeRotation(newRotL);
	}

	//Update the right mesh to its open rotation over time
	if (RMesh->GetComponentRotation() != ROpenRotation)
	{
		FRotator newRotR = FMath::Lerp(RMesh->GetRelativeRotation(), ROpenRotation, _DeltaTime);
		RMesh->SetRelativeRotation(newRotR);
	}
}

void ADoubleDoorCastle::CloseDoor(float _DeltaTime)
{
	//Update the left mesh to its closed rotation over time
	if (LMesh->GetComponentRotation() != LClosedRotation)
	{
		FRotator newRotL = FMath::Lerp(LMesh->GetRelativeRotation(), LClosedRotation, _DeltaTime);
		LMesh->SetRelativeRotation(newRotL);
	}

	//Update the right mesh to its closed rotation over time
	if (RMesh->GetComponentRotation() != RClosedRotation)
	{
		FRotator newRotR = FMath::Lerp(RMesh->GetRelativeRotation(), RClosedRotation, _DeltaTime);
		RMesh->SetRelativeRotation(newRotR);
	}
}

void ADoubleDoorCastle::CheckKeyDoor_Implementation(APlayerCharacter* _Player)
{
	if (_Player)
	{
		if (_Player->PickedUpItem)
		{
			APickupableKey* key = Cast<APickupableKey>(_Player->PickedUpItem);

			//If the player has a key, allow them to interact with the door
			if (key)
			{
				_Player->CanInteract = true;
			}

			//If they are interacting
			if (_Player->IsInteracting)
			{
				//Destroy the key and make the player unable to interact
				_Player->PickedUpItem->Destroy();
				_Player->PickedUpItem = nullptr;
				_Player->CanInteract = false;

				//Play the key inset sound
				if (InsertSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), InsertSound, GetActorLocation());
				}
				//Increment the amount of keys inserted
				KeysInserted++;
			}
		}

		//If the keys inserted is greater or equal to the amount of keys required
		if (KeysInserted >= KeysRequired)
		{
			//Open the door
			IsOpen = true;
			//Play the door open sound
			if (OpenSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
			}
		}

	}
}

void ADoubleDoorCastle::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		//If the 1st overlapping player is null
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				//Increment the number of overlapping player
				NumOfOverlappingPlayers++;
				//Set this player as the first overlapping player
				OverlappingPlayer = playerActor;

			}
		}
		else
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				//Increment the number of overlapping player
				NumOfOverlappingPlayers++;
				//Set this player as the second overlapping player
				OverlappingPlayer2 = playerActor;
			}
		}
	}
}

void ADoubleDoorCastle::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor are players
		if (playerActor)
		{			
			if (OverlappingPlayer)
			{
				//If the leaving player is the 1st overlapping player
				if (OverlappingPlayer == playerActor)
				{
					if (KeysRequired > 0)
					{
						if (OverlappingPlayer->PickedUpItem)
						{
							APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer->PickedUpItem);
							//If the overlapping player has a key and this door requires a key, remove the ability to interact 
							if (key)
							{
								OverlappingPlayer->CanInteract = false;
							}

						}
					}
					OverlappingPlayer = nullptr;
				}
			}

			if (OverlappingPlayer2)
			{
				//If the leaving player is the 2nd overlapping player
				if (OverlappingPlayer2 == playerActor)
				{
					if (KeysRequired > 0)
					{
						if (OverlappingPlayer2->PickedUpItem)
						{
							APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer2->PickedUpItem);
							//If the overlapping player has a key and this door requires a key, remove the ability to interact 
							if (key)
							{
								OverlappingPlayer2->CanInteract = false;
							}

						}
					}

					OverlappingPlayer2 = nullptr;
				}
			}

			//Decrement the amount of overlapping players when they leave
			NumOfOverlappingPlayers--;
		}

	}
}