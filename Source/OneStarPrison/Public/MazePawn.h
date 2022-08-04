// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MazePawn.generated.h"

UCLASS()
class ONESTARPRISON_API AMazePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMazePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	////Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		FTransform EndCameraTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		FTransform CurrentCameraTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		 APlayerCharacter* CurrentPlayer;

	void ExitPawn();
};
