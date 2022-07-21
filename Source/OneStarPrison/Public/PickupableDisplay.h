// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupableDisplay.generated.h"

UCLASS()
class ONESTARPRISON_API APickupableDisplay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupableDisplay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		class UChildActorComponent* PickupableToDisplay;

	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Particles;

	float StartingHeight = 0.0f;

	UPROPERTY(EditAnywhere)
		float FloatScale = 20.0f;

	UPROPERTY(EditAnywhere)
		float RotateScale = 2.0f;


	float RunningTime = 0.0f;
};
