// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleLeverManager.h"
#include "DoubleLever.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

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
}

// Called every frame
void ADoubleLeverManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckLeversOpen();

	if (IsOpen)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Hello s")));
	}
}

void ADoubleLeverManager::CheckLeversOpen_Implementation()
{
	RPCCheckLeversOpen();
}

void ADoubleLeverManager::RPCCheckLeversOpen_Implementation()
{
	for (int i = 0; i < Levers.Num(); i++)
	{
		if (!Levers[i]->IsOpen)
		{
			IsOpen = false;
			return;
		}
	}

	IsOpen = true;
}
