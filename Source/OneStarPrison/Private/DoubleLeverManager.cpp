// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleLeverManager.h"
#include "DoubleLever.h"

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

// Called every frame
void ADoubleLeverManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckLeversOpen())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Hello s")));
	}
}

bool ADoubleLeverManager::CheckLeversOpen()
{
	for (int i = 0; i < Doors.Num(); i++)
	{
		if (!Doors[i]->IsOpen)
		{
			return false;
		}
	}

	return true;
}