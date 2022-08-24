// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PulleyCollector.generated.h"

UCLASS()
class ONESTARPRISON_API APulleyCollector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APulleyCollector();

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

	//Pointer to the platform this pulley is moving
	UPROPERTY(EditAnywhere,Replicated)
		class APulleyPlatform* Platform;

	//Sound that plays when a rock has been added and the platforms move
	UPROPERTY(EditAnywhere, Replicated)
		USoundBase* MovingSound;

	//Have the server call the client play sound
	UFUNCTION(Server, Reliable)
		void ServerPlaySound();
	//Plays the sound for everyone
	UFUNCTION(NetMulticast, Reliable)
		void ClientPlaySound();

	//The starting height of the pulley
	UPROPERTY(VisibleAnywhere, Replicated)
		int StartingHeight = 0;
	//Target height of the pulley
	UPROPERTY(EditAnywhere, Replicated)
		int TargetHeight = 0;

	//The total amount of rocks in this box collision
	UPROPERTY(VisibleAnywhere, Replicated)
		int RockCount = 0;

	//Height to increase the platform by for each rock
	UPROPERTY(EditAnywhere, Replicated)
		int HeightPerRock = 50;

	//The rate and which the platform and the pulley collector moves
	UPROPERTY(EditAnywhere)
		float MoveSpeed = 10.0f;

	//Move the platform and the pulley based on their target height
	void MovePlatform(float _DeltaTime);

	//Update their target position based on the rock count
	void UpdateTargetPos();
};
