// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupableBook.h"
#include "SecretBookshelf.generated.h"

UCLASS()
class ONESTARPRISON_API ASecretBookshelf : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecretBookshelf();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USoundBase* CompleteSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Book1Position;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Book2Position;

	UPROPERTY(EditAnywhere, Replicated)
		FVector OpenPosition = FVector(0, 0, 0);

	class APlayerCharacter* OverlappingPlayer = nullptr;

	UPROPERTY(EditAnywhere)
		bool Book1Inserted = false;

	UPROPERTY(EditAnywhere)
		bool Book2Inserted = false;

	//Check if book is inserted
	bool CheckInsertedBook();

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
