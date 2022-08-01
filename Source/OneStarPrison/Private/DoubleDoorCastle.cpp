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
}


// Called every frame
void ADoubleDoorCastle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		OpenDoor(DeltaTime);
		return;
	}

	if (KeysRequired > 0)
	{
		CheckKeyDoor(OverlappingPlayer);
		CheckKeyDoor(OverlappingPlayer2);
		return;
	}

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
	if (LMesh->GetComponentRotation() != LOpenRotation)
	{
		FRotator newRotL = FMath::Lerp(LMesh->GetRelativeRotation(), LOpenRotation, _DeltaTime);
		LMesh->SetRelativeRotation(newRotL);
	}

	if (RMesh->GetComponentRotation() != ROpenRotation)
	{
		FRotator newRotR = FMath::Lerp(RMesh->GetRelativeRotation(), ROpenRotation, _DeltaTime);
		RMesh->SetRelativeRotation(newRotR);
	}
}

void ADoubleDoorCastle::CheckKeyDoor_Implementation(APlayerCharacter* _Player)
{
	RPCCheckKeyDoor(_Player);
}

void ADoubleDoorCastle::RPCCheckKeyDoor_Implementation(APlayerCharacter* _Player)
{
	if (_Player)
	{
		if (_Player->PickedUpItem)
		{
			APickupableKey* key = Cast<APickupableKey>(_Player->PickedUpItem);

			if (key)
			{
				_Player->CanInteract = true;		
			}

			if (_Player->IsInteracting)
			{
				_Player->PickedUpItem->Destroy();
				_Player->PickedUpItem = nullptr;
				_Player->CanInteract = false;
				if (InsertSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), InsertSound, GetActorLocation());
				}
				KeysInserted++;
			}
		}
		if (KeysInserted >= KeysRequired)
		{
			IsOpen = true;
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
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				NumOfOverlappingPlayers++;
				OverlappingPlayer = playerActor;

			}
		}
		else
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				NumOfOverlappingPlayers++;
				OverlappingPlayer2 = playerActor;
			}
		}
	}
}

void ADoubleDoorCastle::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		//if (OverlappingPlayer != nullptr)
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		if (playerActor)
		{
			if (OverlappingPlayer)
			{
				if (OverlappingPlayer == playerActor)
				{
					if (KeysRequired > 0)
					{
						if (OverlappingPlayer->PickedUpItem)
						{
							APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer->PickedUpItem);
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
				if (OverlappingPlayer2 == playerActor)
				{
					if (KeysRequired > 0)
					{
						if (OverlappingPlayer2->PickedUpItem)
						{
							APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer2->PickedUpItem);
							if (key)
							{
								OverlappingPlayer2->CanInteract = false;
							}

						}
					}

					OverlappingPlayer2 = nullptr;
				}
			}


			NumOfOverlappingPlayers--;
		}

	}
}