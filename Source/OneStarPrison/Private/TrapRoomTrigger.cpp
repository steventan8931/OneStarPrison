// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapRoomTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include "TrapRoomWalls.h"
#include "DoubleLeverManager.h"

// Sets default values
ATrapRoomTrigger::ATrapRoomTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapRoomTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ATrapRoomTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < BreakableBarrels.Num(); i++)
	{
		if (BreakableBarrels[i])
		{
			BarrelTransforms.Add(BreakableBarrels[i]->GetTransform());
			ActorToSpawn = BreakableBarrels[i]->GetClass();

			if (BreakableBarrels[i]->ActorToSpawn)
			{
				ActorHiddenInBarrel = BreakableBarrels[i]->ActorToSpawn;
			}
		}

	}
}

void ATrapRoomTrigger::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATrapRoomTrigger, Triggered);
	DOREPLIFETIME(ATrapRoomTrigger, RoomWalls);
	DOREPLIFETIME(ATrapRoomTrigger, DoubleDoorManager);

	DOREPLIFETIME(ATrapRoomTrigger, BreakableBarrels);
	DOREPLIFETIME(ATrapRoomTrigger, BarrelTransforms);
}

// Called every frame
void ATrapRoomTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Triggered)
	{
		if (OverlappingPlayer)
		{
			if (OverlappingPlayer->HitByWallCount >= 2)
			{
				for (int i = 0; i < RoomWalls.Num(); i++)
				{
					RoomWalls[i]->IsOpen = false;
				}

				for (int i = 0; i < BreakableBarrels.Num(); i++)
				{
					if (BreakableBarrels[i])
					{
						BreakableBarrels[i]->Destroy();
					}
				}
				BreakableBarrels.Empty();

				if (ActorToSpawn)
				{

					if (HasAuthority())
					{
						for (int i = 0; i < BarrelTransforms.Num(); i++)
						{
							ABreakable* barrel = Cast<ABreakable>(SpawnActor(BarrelTransforms[i]));

							if (barrel)
							{
								BreakableBarrels.Add(barrel);
							}

						}

						int KeyBarrel = FMath::RandRange(0, BreakableBarrels.Num() - 1);

						BreakableBarrels[KeyBarrel]->ActorToSpawn = ActorHiddenInBarrel;
					}


				}

				OverlappingPlayer->IsDead = true;
				OverlappingPlayer->DeathTimerCounter = OverlappingPlayer->DeathTimer / 2;
				if (OverlappingPlayer->PickedUpItem)
				{
					OverlappingPlayer->PickedUpItem->Destroy();
					OverlappingPlayer->PickedUpItem = nullptr;
				}
				OverlappingPlayer = false;
				Triggered = false;
			}
		}
	}
}

void ATrapRoomTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
		if (playerActor)
		{
			OverlappingPlayer = playerActor;

			if (!Triggered)
			{
				GEngine->AddOnScreenDebugMessage(-1, 11.0f, FColor::Yellow, TEXT("Triggerd"));
				if (TriggerSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), TriggerSound, GetActorLocation());
				}

				for (int i = 0; i < RoomWalls.Num(); i++)
				{
					RoomWalls[i]->IsOpen = true;
					RoomWalls[i]->PlaySound();

				}

				if (DoubleDoorManager)
				{
					if (DoubleDoorManager->IsOpen)
					{
						DoubleDoorManager->ResetDoors();
					}
				}

				Triggered = true;
			}

		}
	}
}


AActor* ATrapRoomTrigger::SpawnActor(FTransform _Transform)
{
	return(GetWorld()->SpawnActor<AActor>(ActorToSpawn, _Transform));
}

