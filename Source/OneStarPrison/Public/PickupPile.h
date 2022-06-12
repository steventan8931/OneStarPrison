// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupPile.generated.h"

UCLASS()
class ONESTARPRISON_API APickupPile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupPile();

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

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
		int PickupsToSpawn = 3;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AActor>ActorToSpawn;

	UPROPERTY(VisibleAnywhere)
		TArray<class AActor*> ListOfPickups;

	UFUNCTION(NetMulticast, Unreliable)
	void CheckForPickUp();
	UFUNCTION(Server, Unreliable)
	void RPCCheckForPickUp();


	AActor* SpawnActor();

};
