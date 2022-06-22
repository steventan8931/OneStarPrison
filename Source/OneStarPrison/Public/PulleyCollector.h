// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PulleyCollector.generated.h"

UCLASS()
class ONESTARPRISON_API APulleyCollector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APulleyCollector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USoundBase* MovingSound;

	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
		class APulleyPlatform* Platform;

	UPROPERTY(VisibleAnywhere)
		int StartingHeight = 0;

	UPROPERTY(EditAnywhere)
		int TargetHeight = 0;

	UPROPERTY(VisibleAnywhere)
		int RockCount = 0;

	UPROPERTY(EditAnywhere)
		int HeightPerRock = 50;

	UPROPERTY(EditAnywhere)
		float MoveSpeed = 10.0f;

	void MovePlatform(float _DeltaTime);

	void UpdateTargetPos();

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
