// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathRock.generated.h"

UCLASS()
class ONESTARPRISON_API ADeathRock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeathRock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	//The speed the rock goes to reach the end position
	UPROPERTY(EditAnywhere)
		float MoveSpeed = 0.5f;
public:
	//The end position the rock is trying to reach
	UPROPERTY(VisibleAnywhere, Replicated)
		FVector EndPosition;

	//Incremental timer before it checks to destroy
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DeathTime = 0.0f;
	//Time before it destroys itself
	float DeathTimer = 0.0f;
	

};
