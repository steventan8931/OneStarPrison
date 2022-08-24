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

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	//Actor type of the blueprint to spawn
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AActor>ActorToSpawn;

	//Array of actors spawned
	UPROPERTY(VisibleAnywhere, Replicated)
		TArray<class AActor*> ListOfPickups;

	//Has the server check the pick up on the client
	UFUNCTION(Server, Unreliable)
		void RPCCheckForPickUp();
	//Checks for pick up on the client
	UFUNCTION(Client, Unreliable)
	void CheckForPickUp();

	//Spawns the pickupables on the server
	UFUNCTION(Server, Unreliable)
		void ServerSpawn();

	//Spawns specified actor
	AActor* SpawnActor();

};
