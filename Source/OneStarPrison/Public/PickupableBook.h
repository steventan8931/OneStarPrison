// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickupable.h"
#include "PickupableBook.generated.h"

/**
 * 
 */
UCLASS()
class ONESTARPRISON_API APickupableBook : public APickupable
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int Code = 0;
};
