// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
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
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* BoxCollision;

	//Open and Closed Positions of the wall
	UPROPERTY(EditAnywhere, Replicated)
		FVector OpenPosition = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, Replicated)
		FVector ClosedPosition = FVector(0, 0, 0);

	//Moving Sound
	UPROPERTY(EditAnywhere)
		USoundBase* OpenSound;

	//Speed the walls will move from closed to open position
	UPROPERTY(EditAnywhere, Replicated)
		float MoveSpeed = 1.0f;

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);

	//Transition From Current Position to Closed Position
	void CloseDoor(float _DeltaTime);
public:
	//Checks whether the walls are moving or not
	UPROPERTY(EditAnywhere, Replicated)
		bool IsOpen = false;

	//Play open sound
	void PlaySound();
};
