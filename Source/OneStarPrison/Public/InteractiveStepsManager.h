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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TArray<AInteractiveSteps*> ListOfInteractiveSteps;

	UPROPERTY(EditAnywhere)
		TArray<AInteractiveSteps*> CurrentSteps;

	UPROPERTY(EditAnywhere)
		bool IsStepOpen = false;

	//Set the current step that should be open
	void SetOpenStep(TArray<AInteractiveSteps*> &_CurrentStep);

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);

	//Transition From Current Position to Open Position
	void CloseDoor(float _DeltaTime);
};
