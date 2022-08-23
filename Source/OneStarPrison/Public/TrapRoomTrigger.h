// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapRoomWalls.h"
#include "Breakable.h"
#include "TrapRoomTrigger.generated.h"

UCLASS()
class ONESTARPRISON_API ATrapRoomTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapRoomTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Component
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	//Sound that plays when the player overlaps
	UPROPERTY(EditAnywhere)
		USoundBase* TriggerSound;

	//An array of walls that are being moved when this is triggered
	UPROPERTY(EditAnywhere, Replicated)
		TArray<ATrapRoomWalls*> RoomWalls;

	//For Reseting of Puzzle
	//Array of Barrels in Trap room
	UPROPERTY(EditAnywhere, Replicated)
		TArray<ABreakable*> BreakableBarrels;
	//Array of Barrel Transforms
	UPROPERTY(Replicated)
		TArray<FTransform> BarrelTransforms;
	//The type of actor to spawn
	TSubclassOf<class AActor>ActorToSpawn;
	//The type actor that was hidden in barrel
	TSubclassOf<class AActor> ActorHiddenInBarrel;

	//Spawns actor at given transform
	AActor* SpawnActor(FTransform _Transform);

	//Player Interactions
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer;
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer2;

	//Checks given player's hitbywall count 
	void CheckPlayerHit(APlayerCharacter* _Player);
	//Repsawns both players to their last checkpoint
	void RespawnPlayers(APlayerCharacter* _Player);

public:
	//Checks whether a player has overlapped
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	bool Triggered = false;

	//Checks whether a player hit by wall count is >= 2
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	bool IsPlayerHit = false;
};
