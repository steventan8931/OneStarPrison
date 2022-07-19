// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoubleLeverManager.generated.h"

UCLASS()
class ONESTARPRISON_API ADoubleLeverManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoubleLeverManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TArray<ADoubleLever*> Doors;

	bool CheckLeversOpen();

	UPROPERTY(VisibleAnywhere)
	bool IsOpen = false;
};
