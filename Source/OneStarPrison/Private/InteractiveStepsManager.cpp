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

	DOREPLIFETIME(AInteractiveStepsManager, CloseTimer);
	DOREPLIFETIME(AInteractiveStepsManager, CloseDelay);
}


// Called every frame
void AInteractiveStepsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsStepOpen)
	{
		OpenDoor(DeltaTime);
		//CloseTimer = 0.0f;
	}
	else
	{
		//CloseTimer += DeltaTime;
		//if (CloseTimer >= CloseDelay)
		{
			CloseDoor(DeltaTime);
			//CloseTimer = 0.0f;
		}

	}

}

void AInteractiveStepsManager::SetOpenStep(TArray<AInteractiveSteps*>  &_CurrentStep)
{
	CurrentSteps = _CurrentStep;
}


void AInteractiveStepsManager::OpenDoor(float _DeltaTime)
{
	//for (int Index = 0; Index != ListOfInteractiveSteps.Num(); ++Index)
	//{
	//	AInteractiveSteps* step = Cast<AInteractiveSteps>(ListOfInteractiveSteps[Index]);
	//	if (step)
	//	{
	//		for (int i = 0; i < CurrentSteps.Num(); i++)
	//		{
	//			if (step == CurrentSteps[i])
	//			{
	//				step->IsOpen = true;
	//			}
	//			else
	//			{
	//				step->IsOpen = false;
	//			}
	//		}

	//	}		
	//}

	for (int i = 0; i < ListOfInteractiveSteps.Num(); i++)
	{
		if (ListOfInteractiveSteps[i])
		{
			ListOfInteractiveSteps[i]->IsOpen = false;
		}
	
	}

	for (int i = 0; i < CurrentSteps.Num(); i++)
	{
		CurrentSteps[i]->IsOpen = true;
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

