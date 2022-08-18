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

	//To check whether the armor piece should teleport back when dropped
	UPROPERTY(EditAnywhere)
		bool OnDisplay = false;

	//cache for when it has been picked up
	bool TakenFromDisplay = false;
	//Cache Starting Parent Actor
	AActor* ParentActor;
	//Cache Starting Transform
	FTransform cacheTransform;

	//Time before it is reset
	UPROPERTY(EditAnywhere)
		float TimeBeforeItRespawns = 5.0f;
	//Timer to count up to its reset
	float Timer = 0.0f;

	//Sound to play when it is reset to starting position
	UPROPERTY(EditAnywhere)
		USoundBase* ReturnToDisplaySound;
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
