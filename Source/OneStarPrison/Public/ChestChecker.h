// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.h"
#include "ChestChecker.generated.h"

UCLASS()
class ONESTARPRISON_API AChestChecker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChestChecker();

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
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	//Sound that plays when a chest is inserted
	UPROPERTY(EditAnywhere)
		USoundBase* InsertSound;

	//Scene Transform for the position
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* ChestPosition;

	//Door To Open
	UPROPERTY(EditAnywhere, Replicated)
		TArray<ADoor*> Door;

	//Player Interaction
	UPROPERTY(Replicated)
		class APlayerCharacter* OverlappingPlayer = nullptr;
public:

	//Checks whether the chest has been inserted
	UPROPERTY(Replicated, BlueprintReadWrite)
		bool ChestInserted = false;
};
