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

	//HUD Class to add to viewport
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	class UUserWidget* CurrentWidget;

	void InteractPopUp();

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
