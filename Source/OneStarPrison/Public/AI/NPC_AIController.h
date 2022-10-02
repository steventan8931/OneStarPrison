// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "NPC_AIController.generated.h"

UCLASS()
class ONESTARPRISON_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()
public:	
	ANPC_AIController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	void OnPossess(APawn* const CharacterPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> ItemInterested;
	class AActor* GetInterestedItem();
	
	void SetAlert(bool IsAlert);

private:
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

	float SightRadius = 500.0f;
	
	float LoseSightRadius = 25.0f;
	
	float PeripheralVisionAngleDegrees = 90.0f;
	
	float HearingRange = 3000.0f;
	
	bool CanHearPlayer;
	bool CanHearRock;
	bool CanHearBone;

	class UAISenseConfig_Sight* SightConfig;

	class UAISenseConfig_Hearing* HearingConfig;

	UFUNCTION()
	void OnUpdated(TArray<AActor*> const& UpdatedActors);

	void SetupPerceptionSystem();
	
};
