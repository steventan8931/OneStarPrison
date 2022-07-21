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
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	bool SoundPlayed = false;
	float SoundPlayDelay = 0.5f;
	float SoundTimer = 0.5f;


	UPROPERTY(EditAnywhere)
		class USoundBase* OnSound;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MovableMesh;

	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleOpenRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleClosedRotation = FRotator(0, 0, 0);

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, Replicated)
		TArray<class AInteractiveSteps*>  LinkedSteps;

	UPROPERTY(EditAnywhere, Replicated)
		class AInteractiveStepsManager* StepsManager;

	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
