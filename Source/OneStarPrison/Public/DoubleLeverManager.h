// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Replicated)
		TArray<ADoubleLever*> Levers;

	UPROPERTY(EditAnywhere, Replicated)
		TArray<ADoor*> Doors;

	UFUNCTION(NetMulticast, Unreliable)
	void CheckLeversOpen();

	UFUNCTION(Server, Unreliable)
	void RPCCheckLeversOpen();

	UFUNCTION(NetMulticast, Unreliable)
		void ResetDoors();

	UFUNCTION(Server, Unreliable)
		void RPCResetDoors();

	UPROPERTY(VisibleAnywhere, Replicated)
		bool IsOpen = false;

	bool SoundPlayed = false;

};
