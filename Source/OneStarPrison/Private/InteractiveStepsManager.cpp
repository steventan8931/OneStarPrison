// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveStepsManager.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AInteractiveStepsManager::AInteractiveStepsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractiveStepsManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < ListOfInteractiveSteps.Num(); i++)
	{
		if (ListOfInteractiveSteps[i])
		{
			ListOfInteractiveSteps[i]->CloseDelay = CloseDelay;
		}

	}
}

void AInteractiveStepsManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveStepsManager, ListOfInteractiveSteps);
	DOREPLIFETIME(AInteractiveStepsManager, CurrentSteps);

	DOREPLIFETIME(AInteractiveStepsManager, CloseDelay);
}


// Called every frame
void AInteractiveStepsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsStepOpen)
	{
		OpenStep(DeltaTime);
	}
	else
	{
		CloseStep(DeltaTime);
	}

}

//Updates the current array of steps 
void AInteractiveStepsManager::SetOpenStep(TArray<AInteractiveSteps*>  &_CurrentStep)
{
	CurrentSteps = _CurrentStep;
}


void AInteractiveStepsManager::OpenStep(float _DeltaTime)
{
	//Iterate through all the steps are set them to not open
	for (int i = 0; i < ListOfInteractiveSteps.Num(); i++)
	{
		if (ListOfInteractiveSteps[i])
		{
			ListOfInteractiveSteps[i]->IsOpen = false;
		}
	
	}

	//Iterate through the current steps are set them to open
	for (int i = 0; i < CurrentSteps.Num(); i++)
	{
		CurrentSteps[i]->IsOpen = true;
	}
}

void AInteractiveStepsManager::CloseStep(float _DeltaTime)
{
	//Iterate through all the steps and set them to not open
	for (int i = 0; i < ListOfInteractiveSteps.Num(); i++)
	{
		AInteractiveSteps* step = Cast<AInteractiveSteps>(ListOfInteractiveSteps[i]);
		if (step)
		{
			step->IsOpen = false;
		}
	}
}

