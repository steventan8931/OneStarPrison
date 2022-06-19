// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoubleDoor.generated.h"

UCLASS()
class ONESTARPRISON_API ADoubleDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoubleDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
		bool IsOpen = false;

	UPROPERTY(EditAnywhere)
		bool IsKeyOneTimeUse = true;

	UPROPERTY(EditAnywhere, Replicated)
		FVector OpenPosition = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere)
		FVector ClosedPosition = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;
	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer2 = nullptr;

	UPROPERTY(Replicated)
	int NumOfOverlappingPlayers = 0;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);
};
