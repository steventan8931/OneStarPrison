// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class ONESTARPRISON_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USoundBase* MovingSound;

	UPROPERTY(EditAnywhere, Replicated)
		UAudioComponent* AudioComponent;

	UFUNCTION(Server, Unreliable)
		void ServerPlaySound(bool _IsPaused);
	UFUNCTION(NetMulticast, Unreliable)
		void ClientPlaySound(bool _IsPaused);

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MovableMesh;

	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleOpenRotation = FRotator(-50, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		FRotator HandleClosedRotation = FRotator(50, 0, 0);

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, Replicated)
		class ACrankliftPlatform* Platform;

	UPROPERTY(EditAnywhere, Replicated)
		FVector OpenPosition = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
		FVector ClosedPosition = FVector(0, 0, 0);

	UPROPERTY(Replicated)
		bool IsMoving = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float MoveSpeed = 20.0f;

	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	bool FirstFrame = true;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
