// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Breakable.generated.h"

USTRUCT(BlueprintType)
struct FBreakabaleStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UMaterialInstance* Material = nullptr;

	UPROPERTY(Editanywhere)
		int AtHP = 0;
};

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		float ParticleLifeTime = 0.2f;

	float ParticleTimer = 0.0f;

	UPROPERTY(EditAnywhere)
		float TimeToDestroy = 0.1f;

	float DestroyTimer = 0.0f;

	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Particles;

	UPROPERTY(EditAnywhere)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
		USoundBase* BreakSound;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere) 
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
		bool IsBreakableByProjectiles = false;

	UPROPERTY(EditAnywhere)
		int MaxHealth = 0;
	UPROPERTY(VisibleAnywhere)
		int CurrentHealth = 0;
	UPROPERTY(EditAnywhere)
		int DamagePerHit = 1;

	UPROPERTY(EditAnywhere)
		TArray<FBreakabaleStruct> ListOfMaterialsAtHealth;

	bool HitOnce = false;

	void UpdateMaterial();

	class APlayerCharacter* OverlappingPlayer = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor>ActorToSpawn;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor>DeprisToSpawn;

	AActor* SpawnActor(TSubclassOf<class AActor> _Actor);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



};
