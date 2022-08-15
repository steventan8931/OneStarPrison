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
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* OpenSound;

	UPROPERTY(EditAnywhere, Replicated)
		USoundBase* InsertSound;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LMesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RMesh;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		bool IsOpen = false;

	UPROPERTY(EditAnywhere, Replicated)
		FRotator LOpenRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		FRotator ROpenRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		FRotator LClosedRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		FRotator RClosedRotation = FRotator(0, 0, 0);

	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer = nullptr;
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer2 = nullptr;

	UPROPERTY(Replicated)
		int NumOfOverlappingPlayers = 0;

	UPROPERTY(EditAnywhere, Replicated)
		int KeysRequired = 0;

	UPROPERTY(EditAnywhere, Replicated)
		int KeysInserted = 0;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);

	//Transition From Current Position to Close Position
	void CloseDoor(float _DeltaTime);

	//Check Key
	UFUNCTION(NetMulticast, Unreliable)
	void CheckKeyDoor(APlayerCharacter* _Player);
	//Check Key
	UFUNCTION(NetMulticast, Unreliable)
	void RPCCheckKeyDoor(APlayerCharacter* _Player);
};
