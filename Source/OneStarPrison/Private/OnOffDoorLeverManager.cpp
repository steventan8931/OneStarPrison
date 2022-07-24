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

	UpdateDoors();

	if (FirstFrame)
	{
		for (int i = 0; i < OnOffLeverDoors.Num(); i++)
		{
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
	for (int i = 0; i < OnOffLeverDoors.Num(); i++)
	{
		if(OnOffLeverDoors[i].Lever->IsOpen)
		{
			for (int j = 0; j < OnOffLeverDoors.Num(); j++)
			{
				OnOffLeverDoors[i].Doors[j]->IsOpen = true;
			}
		}
		else
		{
			for (int j = 0; j < OnOffLeverDoors.Num(); j++)
			{
				OnOffLeverDoors[i].Doors[j]->IsOpen = false;
			}
		}
	}

}

