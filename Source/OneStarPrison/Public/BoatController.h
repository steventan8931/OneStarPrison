// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoatController.generated.h"

UCLASS()
class ONESTARPRISON_API ABoatController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoatController();

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

	//Sound that plays when the wheel is inserted
	UPROPERTY(EditAnywhere)
		USoundBase* InsertSound;

	//Whether the boat is moving
	UPROPERTY(EditAnywhere, Replicated)
		bool IsMoving = false;

	//Player Interaction
	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//Thethe boat to control
	UPROPERTY(EditAnywhere)
		class AControllableBoat* Boat;

	//Timer and delay for the players interaction
	UPROPERTY(EditAnywhere, Replicated)
		float InteractDelay = 0.3f;
	UPROPERTY(Replicated)
		float InteractTimer = 0.0f;

	//The scale to rotate the boat by for each interaction
	UPROPERTY(EditAnywhere, Replicated)
		float RotationScale = 15.0f;

	//Whether this controller is the anchor
	UPROPERTY(EditAnywhere, Replicated)
		bool IsAnchor = false;

	UPROPERTY(EditAnywhere, Replicated)
		bool IsRowingRight = false;

	UPROPERTY(Replicated)
		FRotator StartingRotation;

	UPROPERTY(EditAnywhere, Replicated)
		FRotator MoveRotation = FRotator(0.0f, 40.0f, 0.0f);

	void RotatePaddle();

public:
	//Whether the anchor has been removed
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		bool AnchorRemoved = false;
};
