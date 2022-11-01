// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawbridgePlatform.generated.h"

UCLASS()
class ONESTARPRISON_API ADrawbridgePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawbridgePlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Component
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	//Open Rotation for the drawbridge mesh
	UPROPERTY(EditAnywhere, Replicated)
		FRotator OpenRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		bool IsTree = false;

	UPROPERTY(EditAnywhere, Replicated)
		float WiggleScale = 1.0f;
public:
	//Checks whether the platform should be updating to its open rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsOpen = false;
	
};
