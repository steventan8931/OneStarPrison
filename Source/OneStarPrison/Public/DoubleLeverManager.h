// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoubleLever.h"
#include "Door.h"
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

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Array of lever
	UPROPERTY(EditAnywhere, Replicated)
		TArray<ADoubleLever*> Levers;

	//Array of doors
	UPROPERTY(EditAnywhere, Replicated)
		TArray<ADoor*> Doors;

	//Checks for whether the levers are open
	UFUNCTION(NetMulticast, Unreliable)
	void CheckLeversOpen();
	UFUNCTION(Server, Unreliable)
	void RPCCheckLeversOpen();

	//Checks whether all the levers are open
	UPROPERTY(VisibleAnywhere, Replicated)
		bool IsOpen = false;

	//Cache to only play sound once
	bool SoundPlayed = false;

};
