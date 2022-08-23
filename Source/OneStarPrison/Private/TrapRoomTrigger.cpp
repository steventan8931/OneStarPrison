// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapRoomTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include "TrapRoomWalls.h"
#include "DoubleLeverManager.h"
#include "Pickupable.h"

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
	
	//Iterates through all the barrels in the array
	for (int i = 0; i < BreakableBarrels.Num(); i++)
	{
		if (BreakableBarrels[i])
		{
			//Adds their transforms to the barrel transform array
			BarrelTransforms.Add(BreakableBarrels[i]->GetTransform());
			//Update the actor to spawn type to be the class of the barrels
			ActorToSpawn = BreakableBarrels[i]->GetClass();

			//If a barrel has an item it spawns when it is destroyed
			if (BreakableBarrels[i]->ActorToSpawn)
			{
				//Set this item as the actor hidden in barrel
				ActorHiddenInBarrel = BreakableBarrels[i]->ActorToSpawn;
				//Remove this actor to spawn from the current barrel
				BreakableBarrels[i]->ActorToSpawn = nullptr;
			}
		}
	}

	//Re-iterate through all the barrels
	for (int i = 0; i < BreakableBarrels.Num(); i++)
	{
		//Randomly choose one barrel to place the key in
		int KeyBarrel = FMath::RandRange(0, BreakableBarrels.Num() - 1);

		//If the barrel chosen barrel and set index is valid
		if (BreakableBarrels[KeyBarrel])
		{
			//Set its actor to spawn on death to be of the same type as the actor hidden in barrel variable
			BreakableBarrels[KeyBarrel]->ActorToSpawn = ActorHiddenInBarrel;
			break;
		}

	}

}

void ATrapRoomTrigger::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATrapRoomTrigger, Triggered);
	DOREPLIFETIME(ATrapRoomTrigger, RoomWalls);

	DOREPLIFETIME(ATrapRoomTrigger, BreakableBarrels);
	DOREPLIFETIME(ATrapRoomTrigger, BarrelTransforms);

	DOREPLIFETIME(ATrapRoomTrigger, OverlappingPlayer);
	DOREPLIFETIME(ATrapRoomTrigger, OverlappingPlayer2);

	DOREPLIFETIME(ATrapRoomTrigger, IsPlayerHit);	
}

// Called every frame
void ATrapRoomTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the trap has been triggered
	if (Triggered)
	{
		//Check if the first overlapping player is hit
		CheckPlayerHit(OverlappingPlayer);
		
		//Check if the second overlapping player is hit
		CheckPlayerHit(OverlappingPlayer2);

		//If a a player has been hit
		if (IsPlayerHit)
		{
			//Respawn Player 1
			RespawnPlayers(OverlappingPlayer);
			//Respawn Player 2
			RespawnPlayers(OverlappingPlayer2);

			//Reset the false to their original positions
			for (int i = 0; i < RoomWalls.Num(); i++)
			{
				RoomWalls[i]->IsOpen = false;
			}

			//Iterate through the breakable barrels array
			for (int i = 0; i < BreakableBarrels.Num(); i++)
			{
				//If the barrel still exists
				if (BreakableBarrels[i])
				{
					//Destroy the barrel
					BreakableBarrels[i]->Destroy();
				}
			}

			//Clear the array of barrels
			BreakableBarrels.Empty();

			if (ActorToSpawn)
			{
				//If it is the server
				if (HasAuthority())
				{
					//Iterate through all the barrel transforms
					for (int i = 0; i < BarrelTransforms.Num(); i++)
					{
						//Spawn barrel at given using index of barrel transform
						ABreakable* barrel = Cast<ABreakable>(SpawnActor(BarrelTransforms[i]));

						if (barrel)
						{
							//Add this barrel to the breakable barrels array
							BreakableBarrels.Add(barrel);
						}

					}

					//Randomly choose a index
					int KeyBarrel = FMath::RandRange(0, BreakableBarrels.Num() - 1);

					//Set barrel of given index to be the barrel which spawns the key
					BreakableBarrels[KeyBarrel]->ActorToSpawn = ActorHiddenInBarrel;
				}

			}

			//Reset Variables
			IsPlayerHit = false;
			Triggered = false;

		}

	}
	
}

void ATrapRoomTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
		//If the overlapping actor is a player
		if (playerActor)
		{
			//If there player 1 isnt already cached
			if (!OverlappingPlayer)
			{
				//Set this player to player 1
				OverlappingPlayer = playerActor;
			}
			else
			{
				if (!OverlappingPlayer2) //If there player 2 isnt already cached
				{
					if (playerActor != OverlappingPlayer) //If the overlapping player is also not player 1
					{
						OverlappingPlayer2 = playerActor; //Set this player to player 2
					}

				}
			}

			//If the trap hasn't already been triggered
			if (!Triggered)
			{
				//Play the trap trigger sound
				if (TriggerSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), TriggerSound, GetActorLocation());
				}

				//Iterate through the walls and have them start moving
				for (int i = 0; i < RoomWalls.Num(); i++)
				{
					RoomWalls[i]->IsOpen = true;
					RoomWalls[i]->PlaySound();
				}

				//Set triggered to true
				Triggered = true;
			}

		}
	}
}

//Spawns actor of type ActorToSpawn, at a given transform
AActor* ATrapRoomTrigger::SpawnActor(FTransform _Transform)
{
	return(GetWorld()->SpawnActor<AActor>(ActorToSpawn, _Transform));
}

//Checks whether the players have been hit by both walls
void ATrapRoomTrigger::CheckPlayerHit(APlayerCharacter* _Player)
{
	if (_Player)
	{
		//If players have been hit by both walls
		if (_Player->HitByWallCount >= 2)
		{
			//Set is player hit to true, which will then reset the puzzle
			IsPlayerHit = true;
		}
	}
}

//Respawns the player
void ATrapRoomTrigger::RespawnPlayers(APlayerCharacter* _Player)
{
	if (_Player)
	{
		//Make the player dead
		_Player->IsDead = true;
		//Make them respawn faster than usual
		_Player->DeathTimerCounter = _Player->DeathTimer / 2;
		//Reset their hit by wall variable
		_Player->HitByWallCount = 0;

		//Remove item from players hand if they are holding an item
		if (_Player->PickedUpItem)
		{
			_Player->PickedUpItem->Destroy();
			_Player->PickedUpItem = nullptr;
		}

		//Set overlapping player variable to null
		_Player = nullptr;	
	}
}

