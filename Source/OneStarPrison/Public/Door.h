// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class ONESTARPRISON_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

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

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		bool IsOpen = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Replicated)
		FVector OpenPosition = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		FVector ClosedPosition = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated, Category = "DoorRotation", BlueprintReadWrite)
		bool IsRotationDoor = false;
	UPROPERTY(EditAnywhere, Replicated, Category = "DoorRotation", BlueprintReadWrite)
		FRotator OpenRotation = FRotator(0, 0, 0);
	UPROPERTY(EditAnywhere, Replicated, Category = "DoorRotation", BlueprintReadWrite)
		FRotator ClosedRotation = FRotator(0, 0, 0);

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);

	//Transition From Current Position to Closed Position
	void CloseDoor(float _DeltaTime);

	//Play open soudn
	void PlaySound();
};
