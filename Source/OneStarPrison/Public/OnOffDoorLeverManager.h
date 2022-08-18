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

	//Array of On/Off Levers
	UPROPERTY(EditAnywhere)
		class AOnOffLever* Lever = nullptr;

	//Array of door
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

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Update the doors to be open/closed based on the levers
	UFUNCTION(NetMulticast, Unreliable)
		void UpdateDoors();
	UFUNCTION(Server, Reliable)
		void RPCUpdateDoors();

	//cache to set values in the first frame
	bool FirstFrame = true;

public:
	//Struct of an array of levers and doors
	UPROPERTY(EditAnywhere, Replicated)
		TArray<FOnOffLeverDoors> OnOffLeverDoors;


};
