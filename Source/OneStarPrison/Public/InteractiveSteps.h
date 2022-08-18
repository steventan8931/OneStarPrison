// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveSteps.generated.h"

UCLASS()
class ONESTARPRISON_API AInteractiveSteps : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveSteps();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Component
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);

	//Transition From Current Position to Closed Position
	void CloseDoor(float _DeltaTime);

	//The open and closed positions for the stepss
	UPROPERTY(EditAnywhere, Replicated)
		FVector OpenPosition = FVector(0, 0, 0);
	UPROPERTY(VisibleAnywhere, Replicated)
		FVector ClosedPosition = FVector(0, 0, 0);

	//The timer to count up to the delay
	UPROPERTY(VisibleAnywhere, Replicated)
		float CloseTimer = 0.0f;
public:
	//Checks whether the steps should be going to their closed or open positions
	UPROPERTY(VisibleAnywhere, Replicated)
		bool IsOpen = false;

	//Delay before it startings closing in again (Set by the manager)
	UPROPERTY(VisibleAnywhere, Replicated)
		float CloseDelay = 0.0f;

};
