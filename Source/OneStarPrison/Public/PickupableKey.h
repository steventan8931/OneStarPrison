// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickupable.h"
#include "PickupableKey.generated.h"

/**
 * 
 */
UCLASS()
class ONESTARPRISON_API APickupableKey : public APickupable
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	//Code to match with the key door
	UPROPERTY(EditAnywhere)
		int KeyCode = 0;
};
