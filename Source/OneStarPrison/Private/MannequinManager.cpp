// Fill out your copyright notice in the Description page of Project Settings.


#include "MannequinManager.h"
#include "Mannequin.h"
#include "Door.h"

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

}

void AMannequinManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMannequinManager, Mannequins);
	DOREPLIFETIME(AMannequinManager, Doors);
	DOREPLIFETIME(AMannequinManager, IsOpen);
}

// Called every frame
void AMannequinManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		for (int i = 0; i < Doors.Num(); i++)
		{
			Doors[i]->IsOpen = true;

			if (!SoundPlayed)
			{
				Doors[i]->PlaySound();
				SoundPlayed = true;
			}
		}

		return;

	}

	CheckMatchingMannequin();

}

void AMannequinManager::CheckMatchingMannequin_Implementation()
{
	RPCCheckMatchingMannequin();

	//Play open sound once 
	if (IsOpen)
	{
		for (int i = 0; i < Doors.Num(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Hello s")));

		}
	}
}

void AMannequinManager::RPCCheckMatchingMannequin_Implementation()
{
	for (int i = 0; i < Mannequins.Num(); i++)
	{
		if (Mannequins[i]->CheckArmorEquipped())
		{
			if (Mannequins[i]->CheckCorrectArmor())
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

	IsOpen = true;
}

