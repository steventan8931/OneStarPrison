// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OnOffLever.generated.h"

UCLASS()
class ONESTARPRISON_API AOnOffLever : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOnOffLever();

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

	//Handle open and closed rotations
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleOpenRotation = FRotator(0, 0, -50);
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleClosedRotation = FRotator(0, 0, 50);

	//Delay before the player can pull the lever again
	UPROPERTY(EditAnywhere)
		float OpenTime = 1.0f;
	//Timer to count up to the delay
	float OpenTimer = 0.0f;

	//Player Interaction
	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//Sound to play when lever is pulled
	UPROPERTY(EditAnywhere)
		USoundBase* OpenSound;
public:	
	//To access the manager to know what doors are linked to the lever
	UPROPERTY(Replicated)
		class AOnOffDoorLeverManager* Manager;
	//Its index in the array of levers
	UPROPERTY(Replicated)
		int IndexInManager = 0;

	UPROPERTY(EditAnywhere, Replicated)
		bool IsOpen = false;
};
