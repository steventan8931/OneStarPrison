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

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//The number to match with the mannequin
	UPROPERTY(EditAnywhere)
		int MannequinNumber = 0;

	//Checks whether it has been attached to a mannequin or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
	bool AttachedToMannequin = false;

	//Enum to choose what armor part it is
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EMannaquinPart> MannequinPart;
};
