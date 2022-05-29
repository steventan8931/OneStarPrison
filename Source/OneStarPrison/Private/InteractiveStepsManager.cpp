// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveStepsManager.h"

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
	
}

// Called every frame
void AInteractiveStepsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsStepOpen)
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}

}

void AInteractiveStepsManager::SetOpenStep(AInteractiveSteps* _CurrentStep)
{
	CurrentStep = _CurrentStep;
}

void AInteractiveStepsManager::OpenDoor(float _DeltaTime)
{
	for (int Index = 0; Index != ListOfInteractiveSteps.Num(); ++Index)
	{
		AInteractiveSteps* step = Cast<AInteractiveSteps>(ListOfInteractiveSteps[Index]);
		if (step)
		{
			if (step == CurrentStep)
			{
				step->IsOpen = true;
			}
			else
			{
				step->IsOpen = false;
			}

		}		
	}
}

void AInteractiveStepsManager::CloseDoor(float _DeltaTime)
{
	for (int Index = 0; Index != ListOfInteractiveSteps.Num(); ++Index)
	{
		AInteractiveSteps* step = Cast<AInteractiveSteps>(ListOfInteractiveSteps[Index]);
		if (step)
		{
			step->IsOpen = false;
		}
	}
}

