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
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		bool IsOpen = false;

	UPROPERTY(EditAnywhere, Replicated)
		FRotator OpenRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		FRotator ClosedRotation = FRotator(0, 0, 0);

	void OpenPlatform(float _DeltaTime);
};
