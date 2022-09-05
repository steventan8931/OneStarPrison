// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Statue.generated.h"

UCLASS()
class ONESTARPRISON_API AStatue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh; 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* AnimalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* LightMesh;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh0;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh1;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh2;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh3;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh4;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh5;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh6;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh7;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh8;

	UPROPERTY(Replicated)
	TArray<UStaticMeshComponent*> Meshes;

	//The scale of normal speed vs crouch speed
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "2", UIMin = "0", UIMax = "2"), Replicated, BlueprintReadWrite)
		int TopRow = 0;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "3", ClampMax = "5", UIMin = "3", UIMax = "5"), Replicated, BlueprintReadWrite)
		int MidRow = 3;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "6", ClampMax = "8", UIMin = "6", UIMax = "8"), Replicated, BlueprintReadWrite)
		int BotRow = 6;

	UPROPERTY(EditAnywhere,Replicated)
		bool IsRandom = true;

	//Array of All Steps
	UPROPERTY(EditAnywhere, Replicated)
		class AStatueManager* Manager = nullptr;

	UPROPERTY(EditAnywhere, Replicated)
		bool IsClone = false;

	UPROPERTY(EditAnywhere,Replicated)
		bool IsChosen = false;

	UFUNCTION(BlueprintCallable)
		void UpdateVisibility();
};
