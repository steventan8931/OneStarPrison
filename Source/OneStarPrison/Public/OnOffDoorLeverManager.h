// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OnOffLever.h"
#include "Door.h"
#include "OnOffDoorLeverManager.generated.h"

USTRUCT(BlueprintType)
struct FOnOffLeverDoors
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		class AOnOffLever* Lever;

	UPROPERTY(EditAnywhere)
		TArray<ADoor*> Doors;
};

UCLASS()
class ONESTARPRISON_API AOnOffDoorLeverManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOnOffDoorLeverManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Replicated)
		TArray<FOnOffLeverDoors> OnOffLeverDoors;

	//Check if both mannequins match
	UFUNCTION(NetMulticast, Unreliable)
		void UpdateDoors();

	UFUNCTION(Server, Reliable)
		void RPCUpdateDoors();

	bool FirstFrame = true;
};
