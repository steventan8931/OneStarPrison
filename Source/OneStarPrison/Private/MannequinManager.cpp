// Fill out your copyright notice in the Description page of Project Settings.


#include "MannequinManager.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AMannequinManager::AMannequinManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMannequinManager::BeginPlay()
{
	Super::BeginPlay();

	//Iterate through the keys
	for (int i = 0; i < Keys.Num(); i++)
	{
		if (Keys[i])
		{
			//Make the keys unable to be picked up
			Keys[i]->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}
}

void AMannequinManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMannequinManager, Mannequins);
	DOREPLIFETIME(AMannequinManager, Doors);
	DOREPLIFETIME(AMannequinManager, IsOpen);
	DOREPLIFETIME(AMannequinManager, Keys);
}

// Called every frame
void AMannequinManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (IsOpen)
	{
		//Iterate through the doors and open them
		for (int i = 0; i < Doors.Num(); i++)
		{
			Doors[i]->IsOpen = true;

			//Play the door open sound only once
			if (!SoundPlayed)
			{
				Doors[i]->PlaySound();
				SoundPlayed = true;
			}
		}
		
		//Don't check for matching mannequins after the doors have been opened
		return;
	}

	//Check whether the mannequins match
	CheckMatchingMannequin();

}

void AMannequinManager::CheckMatchingMannequin_Implementation()
{
	RPCCheckMatchingMannequin();
}

void AMannequinManager::RPCCheckMatchingMannequin_Implementation()
{
	//Iterate through the mannequins
	for (int i = 0; i < Mannequins.Num(); i++)
	{
		Mannequins[i]->CheckArmorEquipped();
		if (Mannequins[i]->MannequinEquiped)
		{
			Mannequins[i]->CheckCorrectArmor();
			if (Mannequins[i]->CorrectArmor)
			{

			}
			else
			{
				IsOpen = false;
				return;
			}
		}
		else
		{
			IsOpen = false;
			return;
		}

	}

	//If all mannequins are matching, allow the keys to be pickupable
	for (int i = 0; i < Keys.Num(); i++)
	{
		Keys[i]->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	//Allow the doors to be opened
	IsOpen = true;
}

