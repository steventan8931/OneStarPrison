// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ControllableBoat.generated.h"

UCLASS()
class ONESTARPRISON_API AControllableBoat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AControllableBoat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	//The speed the boat moves in
	UPROPERTY(EditAnywhere, Replicated)
		float Speed = 100.0f;
	//The start speed 
	UPROPERTY(Replicated)
		float StartSpeed = 100.0f;
	//The start speed 
	UPROPERTY(EditAnywhere, Replicated)
		float MaxSpeed = 200.0f;

	//Sound that plays when the wheel is inserted
	UPROPERTY(EditAnywhere)
		USoundBase* InsertSound;

	UPROPERTY(EditAnywhere)
		USoundBase* CrashSound;

	//Starting Area
	FTransform cacheTransform;

	UPROPERTY(EditAnywhere, Replicated)
		FRotator BuoyancyScale = FRotator(-0.05f, -0.05f, 0.05f);

	UPROPERTY(EditAnywhere, Replicated)
		float BuoyancyHeightScale = 0.05f;

public:
	//Whether the boat is moving
	UPROPERTY(EditAnywhere, Replicated)
		bool IsMoving = false;

	//Insert Item
	UFUNCTION(NetMulticast, Unreliable)
		void InsertItem();

	//Whether the item has been inserted to start the boat
	UPROPERTY(Replicated)
		bool ItemInserted = false;

	//Player Interaction
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer = nullptr;
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer2 = nullptr;

	//Position Snapping
	UPROPERTY(EditAnywhere, Replicated)
		class USceneComponent* LeftRowPosition;
	UPROPERTY(EditAnywhere, Replicated)
		class USceneComponent* RightRowPosition;
};
