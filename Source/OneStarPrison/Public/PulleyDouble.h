// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PulleyDouble.generated.h"

UCLASS()
class ONESTARPRISON_API APulleyDouble : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APulleyDouble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	////Replication
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

	//Pointer to the other pulley
	UPROPERTY(EditAnywhere)
		class APulleyDouble* OtherPlatform;

	//Amount of objects in the pulley
	UPROPERTY(EditAnywhere)
		int RockCount = 0;
	//The height each rock will make the pulley move
	UPROPERTY(EditAnywhere)
		int HeightPerRock = 50;

	//The speed of the pulley updating its position
	UPROPERTY(EditAnywhere)
		float MoveSpeed = 100.0f;

	//Move the pulleys based on their target height
	void MovePlatform(float _DeltaTime);

	//Update the pulleys target height based on the rock count
	void UpdateTargetPos();

	//Sound that plays when it is being moved
	UPROPERTY(EditAnywhere)
		USoundBase* MovingSound;

	//Cache of whether the sound has been played
	UPROPERTY(Replicated)
		bool IsSoundPlaying = false;
	//Timer to count up to the replay sound
	UPROPERTY(Replicated)
		float SoundTimer = 0.0f;
	//Delay till the sound can be played again
	UPROPERTY(Replicated)
		float SoundReplayTimer = 1.0f;

	//Have the server call the client play sound
	UFUNCTION(Server, Reliable)
		void ServerPlaySound();
	//Plays the sound for everyone
	UFUNCTION(NetMulticast, Reliable)
		void ClientPlaySound();
public:
	//Target height of the pulley
	UPROPERTY(VisibleAnywhere)
		int TargetHeight = 0;
	//Maximum height the pulely can go
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxHeight = 0;
	//The starting height of the pulley
	UPROPERTY(VisibleAnywhere)
		int StartingHeight = 0;






};
