// Fill out your copyright notice in the Description page of Project Settings.


#include "MannequinArmor.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Called when the game starts or when spawned
void AMannequinArmor::BeginPlay()
{
	Super::BeginPlay();

	//Set its cache values to its value when the game starts
	cacheTransform = GetActorTransform();
	ParentActor = GetParentActor();
}

void AMannequinArmor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMannequinArmor, AttachedToMannequin);
}

// Called every frame
void AMannequinArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If it is to be reset
	if (OnDisplay)
	{
		//If the a player has picked it up
		if (Player)
		{
			TakenFromDisplay = true;
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