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

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	//Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	//Weightage for the how much the pulley double will go up
	UPROPERTY(EditAnywhere)
		int PulleyWeightage = 7;

	//Cache to check whether the pushable has been pushed to prevent playing of sounds multiple time
	bool HasBeenPushed = false;

	//Cache of starting transform to allow for reseting of object
	UPROPERTY(VisibleAnywhere)
		FTransform StartingTransform;


};
