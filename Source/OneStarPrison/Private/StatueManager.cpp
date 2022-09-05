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

	if (PuzzleCompleted)
	{
		//Iterate through the doors and open them
		for (int i = 0; i < Doors.Num(); i++)
		{
			Doors[i]->IsOpen = true;
		}

		//Don't check for matching mannequins after the doors have been opened
		return;
	}


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

		PuzzleCompleted = true;
		return;
	}


	if (!StatueChosen)
	{
		ResetSteps();

		ResetStatues();

		ChooseStatue();
	}

	CheckCompletion();

	if (StatueFailed)
	{
		TimesCompleted = 0;
		StatueCompleted = false;
		StatueChosen = false;
		StatueFailed = false;
	}

	if (StatueCompleted)
	{
		NextSetTimer += DeltaTime;

		if (NextSetTimer >= NextSetDelay)
		{
			TimesCompleted++;
			StatueChosen = false;
			StatueCompleted = false;
			NextSetTimer = 0.0f;
		}

	}
}

void AStatueManager::ChooseSteps_Implementation()
{
	//Randomly choose one barrel to place the key in
	if (!CurrentStatue)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("statue not set"));
		return;
	}

	int chosenStep = CurrentStatue->TopRow;

	if (ListOfSteps[chosenStep])
	{
		if (!ListOfSteps[chosenStep]->IsChosen)
		{
			ListOfSteps[chosenStep]->IsChosen = true;
		}
	}

	//Randomly choose one barrel to place the key in
	chosenStep = CurrentStatue->MidRow;

	if (ListOfSteps[chosenStep])
	{
		if (!ListOfSteps[chosenStep]->IsChosen)
		{
			ListOfSteps[chosenStep]->IsChosen = true;
		}
	}

	chosenStep = CurrentStatue->BotRow;
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("%lld"), chosenStep));

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
	//Reset steps
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
	//Reset
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

	if (ListOfStatues[chosenStatue])
	{
		if (!ListOfStatues[chosenStatue]->IsChosen)
		{
			ListOfStatues[chosenStatue]->IsChosen = true;
		}

		CurrentStatue = ListOfStatues[chosenStatue];
	}

	ChooseSteps();

	StatueChosen = true;
}

void AStatueManager::CheckCompletion_Implementation()
{
	//Set Manager
	int stepsCompleted = 0;

	for (int i = 0; i < ListOfSteps.Num(); i++)
	{
		if (ListOfSteps[i])
		{
			if (ListOfSteps[i]->IsOn)
			{
				if (ListOfSteps[i]->IsChosen)
				{
					stepsCompleted++;
				}
				else
				{
					StatueFailed = true;
				}
			}
		}
	}

	if (stepsCompleted >= 3)
	{
		StatueCompleted = true;
	}
}
