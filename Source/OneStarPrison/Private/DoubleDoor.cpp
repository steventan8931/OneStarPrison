// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleDoor.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "PickupableKey.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoubleDoor::ADoubleDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	RootComponent = BoxCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoubleDoor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ADoubleDoor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ADoubleDoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoubleDoor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoubleDoor, OpenPosition);
	DOREPLIFETIME(ADoubleDoor, NumOfOverlappingPlayers);
}


// Called every frame
void ADoubleDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		OpenDoor(DeltaTime);
		return;
	}

	if (NumOfOverlappingPlayers == 2)
	{
		if (OverlappingPlayer)
		{
			APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer->PickedUpItem);
			if (key)
			{

				if (IsKeyOneTimeUse)
				{
					OverlappingPlayer->PickedUpItem->Destroy();
					OverlappingPlayer->PickedUpItem = nullptr;
				}

				IsOpen = true;

				if (OpenSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
				}


				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Open"));
			
			}
		}

		if (OverlappingPlayer2)
		{
			APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer2->PickedUpItem);
			if (key)
			{
				//OverlappingPlayer->CanInteract = true;

				//if (OverlappingPlayer->IsInteracting)
				{

					if (key)
					{
						if (IsKeyOneTimeUse)
						{
							OverlappingPlayer2->PickedUpItem->Destroy();
							OverlappingPlayer2->PickedUpItem = nullptr;
						}

						IsOpen = true;

						//OverlappingPlayer->CanInteract = false;

						//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Open"));
					}
				}
			}
		}
	}

}

void ADoubleDoor::OpenDoor(float _DeltaTime)
{
	if (Mesh->GetComponentLocation() != OpenPosition)
	{
		FVector newPos = FMath::Lerp(Mesh->GetComponentLocation(), OpenPosition, _DeltaTime);
		Mesh->SetWorldLocation(newPos);
	}
	//if (GetActorLocation() != OpenPosition)
	//{
	//	FVector newPos = FMath::Lerp(GetActorLocation(), OpenPosition, _DeltaTime);
	//	SetActorLocation(newPos);
	//}
}

void ADoubleDoor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("One Player added"));

			}
		}
		else
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				NumOfOverlappingPlayers++;
				OverlappingPlayer2 = playerActor;
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("One Player added"));
			}
		}
	}
}

void ADoubleDoor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		//if (OverlappingPlayer != nullptr)
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		if (playerActor)
		{
			if (OverlappingPlayer == playerActor)
			{
				OverlappingPlayer = nullptr;
			}
			if (OverlappingPlayer2 == playerActor)
			{
				OverlappingPlayer2 = nullptr;
			}

				NumOfOverlappingPlayers--;
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("One Player left"));
		}
			//OverlappingPlayer->CanInteract = false;

			//OverlappingPlayer = nullptr;
		

	}
}