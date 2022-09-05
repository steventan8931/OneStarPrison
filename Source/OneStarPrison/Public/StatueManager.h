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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* LeftLightMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MidLightMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* RightLightMesh;

	//Array of All Steps
	UPROPERTY(EditAnywhere, Replicated)
		TArray<AStatueSteps*> ListOfSteps;

	//Array of Steps that are being moved
	UPROPERTY(EditAnywhere, Replicated)
		TArray<AStatue*> ListOfStatues;

	UPROPERTY(Replicated)
		AStatue* CurrentStatue = nullptr;

	UPROPERTY(Replicated)
	int TimesCompleted = 0;

	UFUNCTION(Server, Reliable)
	void ChooseSteps();
	UFUNCTION(Server, Reliable)
	void ChooseStatue();

	UFUNCTION(Server, Reliable)
	void ResetSteps();
	UFUNCTION(Server, Reliable)
	void ResetStatues();

	UFUNCTION(Server, Reliable)
	void CheckCompletion();

	UPROPERTY(Replicated)
	bool StatueChosen = false;
	UPROPERTY(Replicated)
	bool StatueCompleted = false;
	UPROPERTY(Replicated)
	bool StatueFailed = false;

	UPROPERTY(Replicated)
	float NextSetTimer = 0.0f;
	UPROPERTY(Replicated)
	float NextSetDelay = 1.0f;

	UPROPERTY(Replicated)
	bool PuzzleCompleted = false;

	//Array of doors to open
	UPROPERTY(EditAnywhere, Replicated)
		TArray<ADoor*> Doors;
};
