// Fill out your copyright notice in the Description page of Project Settings.


#include "OnOffDoorLeverManager.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AOnOffDoorLeverManager::AOnOffDoorLeverManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOnOffDoorLeverManager::BeginPlay()
{
	Super::BeginPlay();
	

}

void AOnOffDoorLeverManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOnOffDoorLeverManager, OnOffLeverDoors);
}

// Called every frame
void AOnOffDoorLeverManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Update the doors based on the levers on/off state
	UpdateDoors();

	//In the first frame
	if (FirstFrame)
	{
		//Iterate through the the array of levers
		for (int i = 0; i < OnOffLeverDoors.Num(); i++)
		{
			//Update the array of levers to have a reference to the manager 
			if (HasAuthority())
			{
				OnOffLeverDoors[i].Lever->Manager = this;
				OnOffLeverDoors[i].Lever->IndexInManager = i;
			}

		}
	}

	FirstFrame = false;
}

void AOnOffDoorLeverManager::UpdateDoors_Implementation()
{
	RPCUpdateDoors();
}

void AOnOffDoorLeverManager::RPCUpdateDoors_Implementation()
{
	//Iterate through the array of the struct of lever
	for (int i = 0; i < OnOffLeverDoors.Num(); i++)
	{
		//Check if the lever is open
		if(OnOffLeverDoors[i].Lever->IsOpen)
		{
			//Iterate through the array of door
			for (int j = 0; j < OnOffLeverDoors[i].Doors.Num(); j++)
			{
				//Set the doors to open
				OnOffLeverDoors[i].Doors[j]->IsOpen = true;
			}
		}
		else
		{
			//Iterate through the array of door		
			for (int j = 0; j < OnOffLeverDoors[i].Doors.Num(); j++)
			{
				//Set the doors to closed
				OnOffLeverDoors[i].Doors[j]->IsOpen = false;
			}
		}
	}

}

