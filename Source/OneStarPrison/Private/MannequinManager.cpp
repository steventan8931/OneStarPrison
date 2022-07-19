// Fill out your copyright notice in the Description page of Project Settings.


#include "MannequinManager.h"
#include "Mannequin.h"

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

// Called every frame
void AMannequinManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckMatchingMannequin())
	{
		GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Hello s")));
	}
}

bool AMannequinManager::CheckMatchingMannequin()
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
				return false;
			}
		}
		else
		{
			return false;
		}

	}

	return true;
}

