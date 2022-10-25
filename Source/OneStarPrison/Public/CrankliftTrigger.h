// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrankliftTrigger.generated.h"

UCLASS()
class ONESTARPRISON_API ACrankliftTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrankliftTrigger();

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

	//Audio
	UPROPERTY(EditAnywhere)
		USoundBase* MovingSound;
	UPROPERTY(EditAnywhere, Replicated)
		UAudioComponent* AudioComponent;

	//Sound Replication
	UFUNCTION(Server, Unreliable)
		void ServerPlaySound(bool _IsPaused);
	UFUNCTION(NetMulticast, Unreliable)
		void ClientPlaySound(bool _IsPaused);

	//Open and closed rotations for the handle
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleOpenRotation = FRotator(0, 0, -50);
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleClosedRotation = FRotator(0, 0, 50);

	//Max and Min heights for the cranklift
	UPROPERTY(EditAnywhere, Replicated)
		float MaxHeight = 0;
	UPROPERTY(EditAnywhere, Replicated)
		float MinHeight = 0;

	//Linked Platform
	UPROPERTY(EditAnywhere, Replicated)
		class ACrankliftPlatform* Platform;

	//Movement
	UPROPERTY(EditAnywhere, Replicated)
		bool IsMovingUp = false;
	UPROPERTY(EditAnywhere)
		float MoveSpeed = 20.0f;

	//Player interaction
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
public:

};
