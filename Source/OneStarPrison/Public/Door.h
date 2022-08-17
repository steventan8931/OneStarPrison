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
private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);

	//Transition From Current Position to Closed Position
	void CloseDoor(float _DeltaTime);
public:
	//Opening Sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* OpenSound;

	//Check whether to open or close the door
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		bool IsOpen = false;

	//Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	//Position of the mesh when opened
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		FVector OpenPosition = FVector(0, 0, 0);
	//Position Of the mesh when rotated
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		FVector ClosedPosition = FVector(0, 0, 0);

	//The rate of which the mesh goes to its end position/rotation
	UPROPERTY(EditAnywhere, Replicated,BlueprintReadWrite)
		float OpenSpeed = 1.0f;

	//Check if it should be a rotation door instead of a location door
	UPROPERTY(EditAnywhere, Replicated, Category = "DoorRotation", BlueprintReadWrite)
		bool IsRotationDoor = false;
	//Rotation of the mesh when opened
	UPROPERTY(EditAnywhere, Replicated, Category = "DoorRotation", BlueprintReadWrite)
		FRotator OpenRotation = FRotator(0, 0, 0);
	//Rotation of the mesh when opened
	UPROPERTY(EditAnywhere, Replicated, Category = "DoorRotation", BlueprintReadWrite)
		FRotator ClosedRotation = FRotator(0, 0, 0);

	//Play open sound once
	void PlaySound();
};
