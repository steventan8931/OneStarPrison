// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatueSteps.h"
#include "Statue.h"
#include "Door.h"
#include "StatueManager.generated.h"

UCLASS()
class ONESTARPRISON_API AStatueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatueManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Set Completion Sound
	UPROPERTY(EditAnywhere)
		USoundBase* OpenSound;

	//Array of All Steps
	UPROPERTY(EditAnywhere, Replicated)
		TArray<AStatueSteps*> ListOfSteps;

	//Array of Statues
	UPROPERTY(EditAnywhere, Replicated)
		TArray<AStatue*> ListOfStatues;

	//Current statue that the player has to match
	UPROPERTY(Replicated)
		AStatue* CurrentStatue = nullptr;

	//Times the player has been completed each statue set
	UPROPERTY(Replicated)
	int TimesCompleted = 0;

	//Chooses steps based on the statue
	UFUNCTION(Server, Reliable)
	void ChooseSteps();
	//Chooses a random statue
	UFUNCTION(Server, Reliable)
	void ChooseStatue();

	//Resets the Chosen Steps
	UFUNCTION(Server, Reliable)
	void ResetSteps();
	//Resets the chosen statue
	UFUNCTION(Server, Reliable)
	void ResetStatues();

	//Check for completion of statue set
	UFUNCTION(Server, Reliable)
	void CheckCompletion();

	//Whether a statue has been chosen
	UPROPERTY(Replicated)
	bool StatueChosen = false;
	UPROPERTY(Replicated)
	//Whether the statue set has been matched
	bool StatueCompleted = false;
	UPROPERTY(Replicated)
	//Whether the statue set was failed to match
	bool StatueFailed = false;

	//Delay and timer to choose next set
	UPROPERTY(Replicated)
	float NextSetTimer = 0.0f;
	UPROPERTY(Replicated)
	float NextSetDelay = 1.0f;

	//Array of doors to open
	UPROPERTY(EditAnywhere, Replicated)
		TArray<ADoor*> Doors;
public:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* LeftLightMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MidLightMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* RightLightMesh;

	//Whether the puzzle has been completed
	UPROPERTY(Replicated)
		bool PuzzleCompleted = false;

};
