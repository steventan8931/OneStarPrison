// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickupable.h"
#include "MannequinArmor.generated.h"

UENUM(BlueprintType)
enum EMannaquinPart
{
	Helmet     UMETA(DisplayName = "Helmet"),
	Armor      UMETA(DisplayName = "Armor"),
	Footwear   UMETA(DisplayName = "Footwear"),
};

UCLASS()
class ONESTARPRISON_API AMannequinArmor : public APickupable
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EMannaquinPart> MannequinPart;
};
