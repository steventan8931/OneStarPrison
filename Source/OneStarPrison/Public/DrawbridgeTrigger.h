// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawbridgeTrigger.generated.h"

UCLASS()
class ONESTARPRISON_API ADrawbridgeTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawbridgeTrigger();

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

	//Opens and closed rotation for the handle
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleOpenRotation = FRotator(-50, 0, 0);
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleClosedRotation = FRotator(50, 0, 0);

	//Sound to play when the drawbridge lowers
	UPROPERTY(EditAnywhere)
		USoundBase* OpenSound;

	//Drawbrdige
	UPROPERTY(EditAnywhere, Replicated)
		class ADrawbridgePlatform* Platform;

	//Checks whether it should be triggered by throwing projectiles or player interaction
	UPROPERTY(EditAnywhere)
		bool IsPlayerInteractable = false;

	//Player Interaction
	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//The speed of which the handle moves
	UPROPERTY(EditAnywhere, Replicated)
		float HandleRotateSpeed = 1.7f;

	//Delay and timer before the handle starts move
	UPROPERTY(EditAnywhere, Replicated)
		float HandleStartDelay = 0.5f;
	UPROPERTY(EditAnywhere, Replicated)
		float HandleStartTimer = 0.0f;

	//The position to snap the player to when interacting
	UPROPERTY(EditAnywhere, Replicated)
		class USceneComponent* HoldPosition;

};
