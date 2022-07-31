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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USoundBase* TriggerSound;

	UPROPERTY(EditAnywhere, Replicated)
		TArray<ATrapRoomWalls*> RoomWalls;

	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer;

	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer2;

	UPROPERTY(EditAnywhere, Replicated)
		class ADoubleLeverManager* DoubleDoorManager;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	bool Triggered = false;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Reseting
	UPROPERTY(EditAnywhere, Replicated)
		TArray<ABreakable*> BreakableBarrels;
	UPROPERTY(Replicated)
		TArray<FTransform> BarrelTransforms;
		TSubclassOf<class AActor>ActorToSpawn;
		TSubclassOf<class AActor> ActorHiddenInBarrel;

	AActor* SpawnActor(FTransform _Transform);

	void CheckPlayerHit(APlayerCharacter* _Player);
	void RespawnPlayers(APlayerCharacter* _Player);

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	bool IsPlayerHit = false;
};
