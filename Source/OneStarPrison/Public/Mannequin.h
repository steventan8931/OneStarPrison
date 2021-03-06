// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MannequinArmor.h"
#include "Mannequin.generated.h"

USTRUCT(BlueprintType)
struct FMannaequinStruct
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		bool HelmetEquipped = false;

	UPROPERTY(EditAnywhere)
		bool ArmorEquipped = false;

	UPROPERTY(EditAnywhere)
		bool FootwearEquipped = false;
};

UCLASS()
class ONESTARPRISON_API AMannequin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMannequin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USoundBase* EquipSound;

	UPROPERTY(EditAnywhere)
		USoundBase* RemoveSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* HelmetPosition;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* ArmorPosition;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* FootwearPosition;

	class APlayerCharacter* OverlappingPlayer = nullptr;

	UPROPERTY(VisibleAnywhere)
		FMannaequinStruct EquippedArmor;

	UPROPERTY(EditAnywhere)
		TArray<AMannequinArmor*> EquippedArray;

	UPROPERTY(Replicated)
		bool MannequinEquiped = false;

	UPROPERTY(Replicated)
		bool CorrectArmor = false;

	UPROPERTY(EditAnywhere)
		int MannequinNumber = 0;

	//Check if all armor has been added
	UFUNCTION(Client, Unreliable)
	void CheckArmorEquipped();
	UFUNCTION(Server, Unreliable)
	void RPCCheckArmorEquipped();

	//Check if correct armor is equipped
	UFUNCTION(Client, Unreliable)
	void CheckCorrectArmor();
	UFUNCTION(Server, Unreliable)
	void RPCCheckCorrectArmor();

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

