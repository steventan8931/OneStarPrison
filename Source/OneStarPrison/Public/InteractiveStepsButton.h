// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveStepsButton.generated.h"

UCLASS()
class ONESTARPRISON_API AInteractiveStepsButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveStepsButton();

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
		class UStaticMeshComponent* MovableMesh;
	UPROPERTY(VisibleAnywhere, Replicated)
		class UBoxComponent* BoxCollision;

	//Lever handles open and closed rotations
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleOpenRotation = FRotator(0, 0, 0);
	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleClosedRotation = FRotator(0, 0, 0);

	//Player interaction
	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//Cache to play sound only once
	bool SoundPlayed = false;
	//Delay before the sound can be played again
	float SoundPlayDelay = 0.5f;
	//Timer to count up to the delay
	float SoundTimer = 0.5f;

	//Opening sound 
	UPROPERTY(EditAnywhere)
		class USoundBase* OnSound;

public:

	//The array of steps controlled by this lever
	UPROPERTY(EditAnywhere, Replicated)
		TArray<class AInteractiveSteps*>  LinkedSteps;
	//Steps manager
	UPROPERTY(EditAnywhere, Replicated)
		class AInteractiveStepsManager* StepsManager;




};
