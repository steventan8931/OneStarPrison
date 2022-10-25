// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class ONESTARPRISON_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

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

	//Open and closed rotations for the handle
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleOpenRotation = FRotator(-50, 0, 0);
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleClosedRotation = FRotator(50, 0, 0);

	//Linked platform to move
	UPROPERTY(EditAnywhere, Replicated)
		class ACrankliftPlatform* Platform;

	//Starting(Open) and end(Closed) positions for the platform
	UPROPERTY(EditAnywhere, Replicated)
		FVector OpenPosition = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, Replicated)
		FVector ClosedPosition = FVector(0, 0, 0);

	//Sound to play when it is moving
	UPROPERTY(EditAnywhere)
		USoundBase* MovingSound;
	//Audio component to allow to pause/unpause the audio
	UPROPERTY(EditAnywhere, Replicated)
		UAudioComponent* AudioComponent;

	//Updates the sound, by pausing/unpausing
	UFUNCTION(Server, Unreliable)
		void ServerPlaySound(bool _IsPaused);
	UFUNCTION(NetMulticast, Unreliable)
		void ClientPlaySound(bool _IsPaused);

	//Check whether the platform is moving or not
	UPROPERTY(Replicated)
		bool IsOpen = false;

	//Player Interaction
	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	bool FirstFrame = true;

	//Sound the platform is unable to move
	UPROPERTY(EditAnywhere)
		USoundBase* FailSound;

	UPROPERTY(Replicated)
	bool cacheInteract = false;

	UPROPERTY(Replicated)
		FVector cacheOpenPos;
public:
	//The speed of which the platform moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float MoveSpeed = 20.0f;

	//The speed of which the handle moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float HandleRotateSpeed = 1.7f;

	//Delay and timer before the handle starts move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float HandleStartDelay = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float HandleStartTimer = 0.0f;

	//The position to snap the player to when interacting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		class USceneComponent* HoldPosition;

};
