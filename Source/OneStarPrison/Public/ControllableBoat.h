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
	UPROPERTY(EditAnywhere)
		float Speed = 100.0f;

	//Player Interaction
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer = nullptr;
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer2 = nullptr;

	//Sound that plays when the wheel is inserted
	UPROPERTY(EditAnywhere)
		USoundBase* InsertSound;

	//Starting Area
	FTransform cacheTransform;
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
};
