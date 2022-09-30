// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PulleyPlatform.generated.h"

UCLASS()
class ONESTARPRISON_API APulleyPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APulleyPlatform();

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

public:
	//The New Height the platform is trying to reach
	UPROPERTY(VisibleAnywhere, Replicated)
		int TargetHeight = 0;

	//The Maximum Height the platform can go up to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		int MaxHeight = 0;

	//The Height the platform starts off at 
	UPROPERTY(VisibleAnywhere)
	int StartingHeight = 0;

};
