// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveStepsButton.generated.h"

UCLASS()
class ONESTARPRISON_API AInteractiveStepsButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveStepsButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	////Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
		TArray<class AInteractiveSteps*>  LinkedSteps;

	UPROPERTY(EditAnywhere)
		class AInteractiveStepsManager* StepsManager;

	UPROPERTY(VisibleAnywhere, Replicated)
		class APlayerCharacter* OverlappingPlayer = nullptr;

	//Overlap Functions
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Overlap Functions
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//HUD Class to add to viewport
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, Replicated)
		class UUserWidget* CurrentWidget;

	UFUNCTION(Server, Unreliable)
		void ServerInteractPopUp();
	UFUNCTION(Client, Unreliable)
	void InteractPopUp();

	UFUNCTION(Server, Unreliable)
		void ServerRemoveWidget();
	UFUNCTION(Client, Unreliable)
	void RemoveWidget();
};
