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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AttachedToMannequin = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EMannaquinPart> MannequinPart;

	UPROPERTY(EditAnywhere)
		int MannequinNumber = 0;

	//For pickupable display (book)
	UPROPERTY(EditAnywhere)
		bool OnDisplay = false;

	bool TakenFromDisplay = false;

	AActor* ParentActor;

	FTransform cacheTransform;

	float Timer = 0.0f;

	UPROPERTY(EditAnywhere)
		float TimeBeforeItRespawns = 5.0f;

	UPROPERTY(EditAnywhere)
		USoundBase* ReturnToDisplaySound;
};
