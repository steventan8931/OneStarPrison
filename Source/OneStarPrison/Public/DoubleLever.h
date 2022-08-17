// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoubleLever.generated.h"

UCLASS()
class ONESTARPRISON_API ADoubleLever : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoubleLever();

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
		class UStaticMeshComponent* MovableMesh;
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	//Handle closed and open rotation
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleOpenRotation = FRotator(0, 0, -50);
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleClosedRotation = FRotator(0, 0, 50);

	//Player interaction
	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//The timer before it reverts back to closed 
	float OpenTimer = 0.0f;
	//Time the lever stays in its open state
	UPROPERTY(EditAnywhere)
		float OpenTime = 1.0f;

	//Sound to play when pulled
	UPROPERTY(EditAnywhere)
		USoundBase* OpenSound;

public:
	//Checks whether the levers are open or not (to be used by the manager)
	UPROPERTY(VisibleAnywhere)
		bool IsOpen = false;
};
