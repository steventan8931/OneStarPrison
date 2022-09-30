// Fill out your copyright notice in the Description page of Project Settings.


#include "StatueManager.h"
#include "PlayerCharacter.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AStatueManager::AStatueManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeftLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLightMesh"));
	MidLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MidLightMesh"));
	RightLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLightMesh"));

	RootComponent = LeftLightMesh;

	MidLightMesh->SetupAttachment(RootComponent);
	RightLightMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AStatueManager::BeginPlay()
{
	Super::BeginPlay();

	//Set Manager
	for (int i = 0; i < ListOfStatues.Num(); i++)
	{
		if (ListOfStatues[i])
		{
			ListOfStatues[i]->Manager = this;
		}
	}

	//Set Manager
	for (int i = 0; i < ListOfSteps.Num(); i++)
	{
		if (ListOfSteps[i])
		{
			ListOfSteps[i]->Manager = this;
		}
	}

	//Set the meshes of indicators off
	LeftLightMesh->SetVisibility(false);
	MidLightMesh->SetVisibility(false);
	RightLightMesh->SetVisibility(false);
}

void AStatueManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStatueManager, ListOfSteps);
	DOREPLIFETIME(AStatueManager, ListOfStatues);

	DOREPLIFETIME(AStatueManager, StatueChosen);
	DOREPLIFETIME(AStatueManager, StatueCompleted);
	DOREPLIFETIME(AStatueManager, StatueFailed);
	DOREPLIFETIME(AStatueManager, NextSetTimer);
	DOREPLIFETIME(AStatueManager, NextSetDelay);
	DOREPLIFETIME(AStatueManager, PuzzleCompleted);

	DOREPLIFETIME(AStatueManager, CurrentStatue);
	DOREPLIFETIME(AStatueManager, TimesCompleted);
}


// Called every frame
void AStatueManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the puzzle has been completed
	if (PuzzleCompleted)
	{
		//Iterate through the doors and open them
		for (int i = 0; i < Doors.Num(); i++)
		{
			Doors[i]->IsOpen = true;
		}

		//Don't check for statue puzzle after the doors have been opened
		return;
	}

	//Turn on the indicator meshes based on how many times the puzzle has been completed
	switch (TimesCompleted)
	{
	case 0:
		LeftLightMesh->SetVisibility(false);
		MidLightMesh->SetVisibility(false);
		RightLightMesh->SetVisibility(false);
		break;
	case 1: 
		LeftLightMesh->SetVisibility(true);
		MidLightMesh->SetVisibility(false);
		RightLightMesh->SetVisibility(false);
		break;
	case 2:
		LeftLightMesh->SetVisibility(true);
		MidLightMesh->SetVisibility(true);
		RightLightMesh->SetVisibility(false);
		break;
	case 3:
		LeftLightMesh->SetVisibility(true);
		MidLightMesh->SetVisibility(true);
		RightLightMesh->SetVisibility(true);

		//Iterate through the doors play sound
		for (int i = 0; i < Doors.Num(); i++)
		{
			Doors[i]->PlaySound();
		}
		//Set Puzzle to be completed after it has been completed 3 times
		PuzzleCompleted = true;

		return;
	}

	//If the statue hasnt been chosen
	if (!StatueChosen)
	{
		//Reset the chosen steps
		ResetSteps();

		//Reset the statues
		ResetStatues();

		//Choose a statue
		ChooseStatue();
	}

	//Check for completion of the puzzle
	CheckCompletion();

	//If a set has been failed
	if (StatueFailed)
	{
		//Reset all the variables and set the times completed back to 0
		TimesCompleted = 0;
		StatueCompleted = false;
		StatueChosen = false;
		StatueFailed = false;
	}

	//If the set has been completed
	if (StatueCompleted)
	{
		NextSetTimer += DeltaTime;

		//After a delay
		if (NextSetTimer >= NextSetDelay)
		{
			//Play completion sound
			if (OpenSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
			}

			//Increment times completed
			TimesCompleted++;
			//Reset Variables
			StatueChosen = false;
			StatueCompleted = false;
			NextSetTimer = 0.0f;
		}

	}
}

void AStatueManager::ChooseSteps_Implementation()
{
	//If a statue hasnt been chosen
	if (!CurrentStatue)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("statue not set"));
		return;
	}

	//Update the steps to match the statue's step for the top row
	int chosenStep = CurrentStatue->TopRow;

	if (ListOfSteps[chosenStep])
	{
		if (!ListOfSteps[chosenStep]->IsChosen)
		{
			ListOfSteps[chosenStep]->IsChosen = true;
		}
	}
	//Update the steps to match the statue's step for the middle row
	chosenStep = CurrentStatue->MidRow;

	if (ListOfSteps[chosenStep])
	{
		if (!ListOfSteps[chosenStep]->IsChosen)
		{
			ListOfSteps[chosenStep]->IsChosen = true;
		}
	}
	//Update the steps to match the statue's step for the bottom row
	chosenStep = CurrentStatue->BotRow;

	if (ListOfSteps[chosenStep])
	{
		if (!ListOfSteps[chosenStep]->IsChosen)
		{
			ListOfSteps[chosenStep]->IsChosen = true;
		}
	}
}

void AStatueManager::ResetSteps_Implementation()
{
	//Iterate through and reset the steps
	for (int i = 0; i < ListOfSteps.Num(); i++)
	{
		if (ListOfSteps[i])
		{
			ListOfSteps[i]->IsOn = false;
			ListOfSteps[i]->IsChosen = false;
		}
	}
}

void AStatueManager::ResetStatues_Implementation()
{
	//Iterate through and reset the statues
	for (int i = 0; i < ListOfStatues.Num(); i++)
	{
		if (ListOfStatues[i])
		{
			ListOfStatues[i]->IsChosen = false;
		}
	}
}

void AStatueManager::ChooseStatue_Implementation()
{
	//Choose one statue
	int chosenStatue = FMath::RandRange(0, ListOfStatues.Num() - 1);

	//If the statue isn't chosen, set this statue to be the chosen statue
	if (ListOfStatues[chosenStatue])
	{
		if (!ListOfStatues[chosenStatue]->IsChosen)
		{
			ListOfStatues[chosenStatue]->IsChosen = true;
		}

		CurrentStatue = ListOfStatues[chosenStatue];
	}

	//Choose the steps
	ChooseSteps();

	//Set statuechosen to true
	StatueChosen = true;
}

void AStatueManager::CheckCompletion_Implementation()
{
	//Check step completion 
	int stepsCompleted = 0;

	//Iterate through each step
	for (int i = 0; i < ListOfSteps.Num(); i++)
	{
		if (ListOfSteps[i])
		{
			//If the step has been stepped on
			if (ListOfSteps[i]->IsOn)
			{
				//And the step was chosen 
				if (ListOfSteps[i]->IsChosen)
				{
					//Increment the step completed count
					stepsCompleted++;
				}
				else
				{
					//Otherwise fail the puzzle
					StatueFailed = true;
				}
			}
		}
	}

	//If 3 steps correct steps have been stepped on
	if (stepsCompleted >= 3)
	{
		//Complete the set 
		StatueCompleted = true;
	}
}
