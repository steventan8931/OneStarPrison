// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MannequinManager.generated.h"

UCLASS()
class ONESTARPRISON_API AMannequinManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMannequinManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TArray<AMannequin*> Mannequins;

	UPROPERTY(EditAnywhere)
		TArray<ADoor*> Doors;

	UPROPERTY(VisibleAnywhere)
		bool IsOpen = false;

	//Check if both mannequins match
	bool CheckMatchingMannequin();

};
