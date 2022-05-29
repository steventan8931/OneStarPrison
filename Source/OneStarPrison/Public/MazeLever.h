// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeLever.generated.h"

UCLASS()
class ONESTARPRISON_API AMazeLever : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeLever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere)
	//	class UStaticMeshComponent* Mesh;

	//UPROPERTY(VisibleAnywhere)
	//	class UBoxComponent* BoxCollision;

	////HUD Class to add to viewport
	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class UUserWidget> HUDWidgetClass;

	//UPROPERTY(VisibleAnywhere)
	//	class UUserWidget* CurrentWidget;

	//void InteractPopUp();

	////Overlap Functions
	//UFUNCTION()
	//	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	////Overlap Functions
	//UFUNCTION()
	//	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
