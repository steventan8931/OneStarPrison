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

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Component
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	//Position for mannequin equippables
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* HelmetPosition;
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* ArmorPosition;
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* FootwearPosition;

	//The number to match the armor pieces with
	UPROPERTY(EditAnywhere)
		int MannequinNumber = 0;

	//Struct of whether each armor piece is equipped
	UPROPERTY(VisibleAnywhere)
		FMannaequinStruct EquippedArmor;

	//Check if all armor has been added
	UFUNCTION(Server, Unreliable)
		void RPCCheckArmorEquipped();

	//Check if correct armor is equipped
	UFUNCTION(Server, Unreliable)
		void RPCCheckCorrectArmor();

	//Array of currently equipped armor
	UPROPERTY(EditAnywhere)
		TArray<AMannequinArmor*> EquippedArray;

	//Sound that plays when equipping armor piece
	UPROPERTY(EditAnywhere)
		USoundBase* EquipSound;
	//Sound that plays with removing armor pieces
	UPROPERTY(EditAnywhere)
		USoundBase* RemoveSound;

	//Player Intearction
	class APlayerCharacter* OverlappingPlayer = nullptr;

public:	
	//Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	//Checks whether all 3 pieces are equipped
	UPROPERTY(Replicated)
		bool MannequinEquiped = false;

	//Check if all armor has been added
	UFUNCTION(Client, Unreliable)
		void CheckArmorEquipped();

	//Checks whether all 3 pieces are the correct pieces
	UPROPERTY(Replicated)
		bool CorrectArmor = false;

	//Check if correct armor is equipped
	UFUNCTION(Client, Unreliable)
		void CheckCorrectArmor();

};

