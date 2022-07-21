// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Replicated)
		TArray<AMannequin*> Mannequins;

	UPROPERTY(EditAnywhere, Replicated)
		TArray<ADoor*> Doors;

	//Check if both mannequins match
	UFUNCTION(Client, Unreliable)
		void CheckMatchingMannequin();

	UFUNCTION(Server, Unreliable)
		void RPCCheckMatchingMannequin();

	UPROPERTY(VisibleAnywhere, Replicated)
		bool IsOpen = false;

	bool SoundPlayed = false;

};
