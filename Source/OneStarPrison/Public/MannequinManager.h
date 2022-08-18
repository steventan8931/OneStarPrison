// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mannequin.h"
#include "Door.h"
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

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Array of mannequins to check
	UPROPERTY(EditAnywhere, Replicated)
		TArray<AMannequin*> Mannequins;
	//Array of doors to open
	UPROPERTY(EditAnywhere, Replicated)
		TArray<ADoor*> Doors;

	//Array of keys to set pickupable
	UPROPERTY(EditAnywhere, Replicated)
		TArray<APickupable*> Keys;

	//Check if both mannequins match
	UFUNCTION(Client, Unreliable)
		void CheckMatchingMannequin();
	UFUNCTION(Server, Unreliable)
		void RPCCheckMatchingMannequin();

	//Check whether the doors should open or not
	UPROPERTY(VisibleAnywhere, Replicated)
		bool IsOpen = false;

	//cache to play sound only once
	bool SoundPlayed = false;

};
