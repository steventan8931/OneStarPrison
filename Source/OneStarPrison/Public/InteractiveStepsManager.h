// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveSteps.h"
#include "InteractiveStepsManager.generated.h"

UCLASS()
class ONESTARPRISON_API AInteractiveStepsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveStepsManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Array of All Steps
	UPROPERTY(EditAnywhere, Replicated)
		TArray<AInteractiveSteps*> ListOfInteractiveSteps;
	//Array of Steps that are being moved
	UPROPERTY(EditAnywhere, Replicated)
		TArray<AInteractiveSteps*> CurrentSteps;

	//Delay before it startings closing in again
	UPROPERTY(EditAnywhere, Replicated)
		float CloseDelay = 0.0f;

public:
	//Check whether it the current set of steps should be moving
	UPROPERTY(EditAnywhere)
		bool IsStepOpen = false;

	//Set the current step that should be open
	void SetOpenStep(TArray<AInteractiveSteps*>& _CurrentStep);

	//Transition From Current Position to Open Position
	void OpenStep(float _DeltaTime);
	//Transition From Current Position to Closed Position
	void CloseStep(float _DeltaTime);
};
