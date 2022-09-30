// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoubleDoorCastle.generated.h"

UCLASS()
class ONESTARPRISON_API ADoubleDoorCastle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoubleDoorCastle();

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

	//Componennts for the left and right door
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LMesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RMesh;
	//Overlap collision
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);

	//Transition From Current Position to Close Position
	void CloseDoor(float _DeltaTime);

	//Rotation the mesh will have when opened
	UPROPERTY(EditAnywhere, Replicated)
		FRotator LOpenRotation = FRotator(0, 0, 0);
	UPROPERTY(EditAnywhere, Replicated)
		FRotator ROpenRotation = FRotator(0, 0, 0);

	//Rotation the mesh will have when closed
	UPROPERTY(EditAnywhere, Replicated)
		FRotator LClosedRotation = FRotator(0, 0, 0);
	UPROPERTY(EditAnywhere, Replicated)
		FRotator RClosedRotation = FRotator(0, 0, 0);

	//Amount of keys needed to open the door 
	UPROPERTY(EditAnywhere, Replicated)
		int KeysRequired = 0;
	//Total amount of keys inserted (ignore if no keys are required)
	UPROPERTY(EditAnywhere, Replicated)
		int KeysInserted = 0;

	//Sound for key insert (if appliclable)
	UPROPERTY(EditAnywhere, Replicated)
		USoundBase* InsertSound;

	//To cache and check for player interaction with key
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer = nullptr;
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer2 = nullptr;

	//Checks whether two players are in the box collision (For doors without keys)
	UPROPERTY(Replicated)
		int NumOfOverlappingPlayers = 0;

	//Check Key
	UFUNCTION(NetMulticast, Unreliable)
		void CheckKeyDoor(APlayerCharacter* _Player);
public:
	//Sound for the doors opening
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* OpenSound;

	//Check whether the doors should be open or closed
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		bool IsOpen = false;
};
