// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrankliftTrigger.generated.h"

UCLASS()
class ONESTARPRISON_API ACrankliftTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrankliftTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
		class ACrankliftPlatform* Platform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinHeight = 0;

	UPROPERTY(EditAnywhere)
		bool IsMovingUp = false;

	float cacheDeltaTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveSpeed = 20.0f;

	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
