// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Breakable.generated.h"

//USTRUCT(BlueprintType)
//struct FBreakabaleStruct
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere)
//	UMaterialInstance* Material = nullptr;
//
//	UPROPERTY(Editanywhere)
//		int AtHP = 0;
//};

UCLASS()
class ONESTARPRISON_API ABreakable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	//Particle class
	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Particles;
	//Particle Life time
	UPROPERTY(EditAnywhere)
		float ParticleLifeTime = 0.2f;
	float ParticleTimer = 0.0f;

	//Actor life time
	UPROPERTY(EditAnywhere)
		float TimeToDestroy = 0.1f;
	float DestroyTimer = 0.0f;

	//Object hit and break sound
	UPROPERTY(EditAnywhere)
		USoundBase* HitSound;
	UPROPERTY(EditAnywhere)
		USoundBase* BreakSound;

	//Object health and damage per hit
	UPROPERTY(EditAnywhere)
		int MaxHealth = 0;
	UPROPERTY(VisibleAnywhere)
		int CurrentHealth = 0;
	UPROPERTY(EditAnywhere)
		int DamagePerHit = 1;

	//Check if the breakable is to be broken for projectiles or the player
	UPROPERTY(EditAnywhere)
		bool IsBreakableByProjectiles = false;

	//Player interaction
	class APlayerCharacter* OverlappingPlayer = nullptr;

	void UpdateHealth();
public:

	//UPROPERTY(EditAnywhere)
	//	TArray<FBreakabaleStruct> ListOfMaterialsAtHealth;

	//Objects to spawn when the actor is destroyed
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor>ActorToSpawn;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor>DebrisToSpawn;

	//Spawns actor of type
	AActor* SpawnActor(TSubclassOf<class AActor> _Actor);


};
