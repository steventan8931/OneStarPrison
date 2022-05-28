// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyDoor.generated.h"

UCLASS()
class ONESTARPRISON_API AKeyDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
		bool IsOpen = false;

	UPROPERTY(EditAnywhere)
		bool IsKeyOneTimeUse = false;

	UPROPERTY(EditAnywhere, Replicated)
		FVector OpenPosition = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere)
		FVector ClosedPosition = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//HUD Class to add to viewport
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere)
		class UUserWidget* CurrentWidget;

	void InteractPopUp();

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Transition From Current Position to Open Position
	void OpenDoor(float _DeltaTime);
};
