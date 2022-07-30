// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapRoomWalls.generated.h"

UCLASS()
class ONESTARPRISON_API ATrapRoomWalls : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapRoomWalls();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USoundBase* OpenSound;

	UPROPERTY(EditAnywhere, Replicated)
		bool IsOpen = false;

	UPROPERTY(EditAnywhere, Replicated)
		float MoveSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Replicated)
		FVector OpenPosition = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		FVector ClosedPosition = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* BoxCollision;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);

	//Transition From Current Position to Closed Position
	void CloseDoor(float _DeltaTime);

	//Play open soudn
	void PlaySound();
};
