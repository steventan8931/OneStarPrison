// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatueSteps.generated.h"

UCLASS()
class ONESTARPRISON_API AStatueSteps : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatueSteps();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* OnMesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* OffMesh;
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;
public:

	//Sound to play when the drawbridge lowers
	UPROPERTY(EditAnywhere)
		USoundBase* CorrectSound;
	//Sound to play when the drawbridge lowers
	UPROPERTY(EditAnywhere)
		USoundBase* FailSound;

	//Player Interaction
	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//Player Interaction
	UPROPERTY(VisibleAnywhere, Replicated)
		class AStatueManager* Manager = nullptr;

	UPROPERTY(Replicated);
	bool IsOn = false;

	UPROPERTY(Replicated)
		bool IsChosen = false;
};
