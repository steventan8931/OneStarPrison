// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrankliftPlatform.generated.h"

UCLASS()
class ONESTARPRISON_API ACrankliftPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrankliftPlatform();

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
	//Collision
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxCollision;

	//Player Interaction
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//Reference to the weighted object
	UPROPERTY(VisibleAnywhere, Replicated)
		class APickupable* OverlappingWeight = nullptr;

	//Whether the object should move side to side
	UPROPERTY(EditAnywhere, Replicated)
		bool IsSideToSide = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Component
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	//Whether this platform can only move one object at a time
	UPROPERTY(EditAnywhere, Replicated)
		bool IsWeightDependent = false;

	//Used when the cranklift is weight dependent
	UPROPERTY(Replicated)
		bool CanMove = true;

};
