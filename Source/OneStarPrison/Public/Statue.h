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

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components for each step
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

	//Mesh Array
	UPROPERTY(Replicated)
		TArray<UStaticMeshComponent*> Meshes;

	//If the statue should be randomized
	UPROPERTY(EditAnywhere, Replicated)
		bool IsRandom = true;

	//Whether the statue is a clone of another
	UPROPERTY(EditAnywhere, Replicated)
		bool IsClone = false;

	//Update visiblity of statue to match the manager
	UFUNCTION(BlueprintCallable)
		void UpdateVisibility();

public:
	//Statue support mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;
	//Animal statue Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* AnimalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* LightMesh;

	//The Min and Max Values for each row (index for the mesh array)
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "2", UIMin = "0", UIMax = "2"), Replicated, BlueprintReadWrite)
		int TopRow = 0;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "3", ClampMax = "5", UIMin = "3", UIMax = "5"), Replicated, BlueprintReadWrite)
		int MidRow = 3;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "6", ClampMax = "8", UIMin = "6", UIMax = "8"), Replicated, BlueprintReadWrite)
		int BotRow = 6;

	//Statue Manager
	UPROPERTY(EditAnywhere, Replicated)
		class AStatueManager* Manager = nullptr;

	//Whether  this statue is chosen
	UPROPERTY(EditAnywhere, Replicated)
		bool IsChosen = false;
};
