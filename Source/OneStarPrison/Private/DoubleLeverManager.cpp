// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleLeverManager.h"
#include "DoubleLever.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Door.h"

// Sets default values
ADoubleLeverManager::ADoubleLeverManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoubleLeverManager::BeginPlay()
{
	Super::BeginPlay();

}


void ADoubleLeverManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoubleLeverManager, Levers);
	DOREPLIFETIME(ADoubleLeverManager, Doors);
	DOREPLIFETIME(ADoubleLeverManager, IsOpen);
}

// Called every frame
void ADoubleLeverManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the double levers are both open
	if (IsOpen)
	{
		//Iterate through the array of door
		for (int i = 0; i < Doors.Num(); i++)
		{
			//Set then to be opened
			Doors[i]->IsOpen = true;

			//If the open sound hasn't been played
			if (!SoundPlayed)
			{
				//Play it once
				Doors[i]->PlaySound();
				SoundPlayed = true;
			}
		}

		//Don't Check for if levers are open
		return;

	}

	//Check if the 
	CheckLeversOpen();

}

void ADoubleLeverManager::CheckLeversOpen_Implementation()
{
	RPCCheckLeversOpen();
}

void ADoubleLeverManager::RPCCheckLeversOpen_Implementation()
{
	//Iterate through the array of lever
	for (int i = 0; i < Levers.Num(); i++)
	{
		//If any levers are not open
		if (!Levers[i]->IsOpen)
		{
			//Set the IsOpen bool to false and stop iterating
			IsOpen = false;
			return;
		}
	}

	//If all levers are open, set the IsOpen bool to ture
	IsOpen = true;
}

