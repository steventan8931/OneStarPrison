// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Pickupable.generated.h"

UCLASS()
class ONESTARPRISON_API APickupable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	////Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Sound to play when picked up
	UPROPERTY(EditAnywhere)
		USoundBase* PickupSound;

	//To check whether the armor piece should teleport back when dropped
	UPROPERTY(EditAnywhere)
		bool OnDisplay = false;

	//cache for when it has been picked up
	bool TakenFromDisplay = false;
	//Cache Starting Parent Actor
	AActor* ParentActor;
	//Cache Starting Transform
	FTransform cacheTransform;

	//Time before it is reset
	UPROPERTY(EditAnywhere)
		float TimeBeforeItRespawns = 5.0f;
	//Timer to count up to its reset
	float Timer = 0.0f;

	//Sound to play when it is reset to starting position
	UPROPERTY(EditAnywhere)
		USoundBase* ReturnToDisplaySound;
public:	
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;
	//Projectile Movement 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* ProjectileMovement;

	//Check whether the object has hit something
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool IsInAir = false;

	//To launch the object using its projectile movement
	void Launch(FVector _Velocity);

	//Player Interaction
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		class APlayerCharacter* Player;

	//Plays pick up sound
	void PlayPickupSound();
};
