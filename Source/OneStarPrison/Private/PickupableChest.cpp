// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableChest.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Called when the game starts or when spawned
void APickupableChest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickupableChest::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

	//If the chest is being held by a player, make the player's holding heavy item to true
	if (Player)
	{
		Player->IsHoldingHeavyItem = true;

		if (Player->IsDead)
		{
			TakenFromDisplay = true;
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			//Deatch the pickupable and reset its variables
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			//Make it ignore only the player
			Mesh->SetCollisionProfileName("IgnoreOnlyPawn");
			//Re-enable collision of query and physics
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			//Remove the pointer reference from the item
			Player = nullptr;

			Timer = TimeBeforeItRespawns;
		}
	}

	//If it is to be reset
	if (OnDisplay)
	{
		//If the a player has picked it up
		if (Player)
		{
			TakenFromDisplay = true;
			Timer = 0.0f;
		}
		else
		{
			//If the item has been picked up but no longer attached to a player
			if (TakenFromDisplay)
			{
				//When it hits something
				if (!IsInAir)
				{
					//Increment the timer over time
					Timer += DeltaTime;
				}
			}
		}

		//If the timer has reached the time before it respawns
		if (Timer > TimeBeforeItRespawns)
		{
			//Reset the bool
			TakenFromDisplay = false;

			//If a parent actor exists
			if (ParentActor)
			{
				//Attach to its parent actor
				AttachToActor(ParentActor, FAttachmentTransformRules::SnapToTargetIncludingScale);
			}

			//Resets its transform to its starting transform
			SetActorTransform(cacheTransform);

			//Disable physics
			Mesh->SetSimulatePhysics(false);

			//Play the return sound
			if (ReturnToDisplaySound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReturnToDisplaySound, GetActorLocation());
			}

			//Reset the timer
			Timer = 0.0f;
		}

	}
}