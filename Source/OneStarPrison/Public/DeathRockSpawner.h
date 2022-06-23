// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathRockSpawner.generated.h"

UCLASS()
class ONESTARPRISON_API ADeathRockSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeathRockSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AActor>ActorToSpawn;

	UPROPERTY(EditAnywhere, Replicated)
		float SpawnRate = 5.0f;

	UPROPERTY(EditAnywhere, Replicated)
		FVector EndPosition;

	UPROPERTY(Replicated)
	float SpawnTimer = 0.0f;

	AActor* SpawnActor();

};
