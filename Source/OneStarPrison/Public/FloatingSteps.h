// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingSteps.generated.h"

UCLASS()
class ONESTARPRISON_API AFloatingSteps : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingSteps();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Replicated)
		class UBoxComponent* BoxCollision;

	//The Height of the object when it is not stepped on
	UPROPERTY(Replicated)
		float UpHeight = 0.0f;

	//The Height of the object when it has been stepped on
	UPROPERTY(EditAnywhere, Replicated)
		float DownHeight = 0.0f;

	//Player Interaction
	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//Checks whether the door is opened
	UPROPERTY(VisibleAnywhere, Replicated)
		bool IsPlayerColliding = false;

	//The speed at which the object updates it height
	UPROPERTY(EditAnywhere, Replicated)
		float MoveSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Replicated)
		bool IsSideToSide = false;
public:


};
