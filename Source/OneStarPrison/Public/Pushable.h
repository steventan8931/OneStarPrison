// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pushable.generated.h"

UCLASS()
class ONESTARPRISON_API APushable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APushable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		int PulleyWeightage = 7;

	bool HasBeenPushed = false;

	UPROPERTY(VisibleAnywhere)
		FTransform StartingTransform;

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
