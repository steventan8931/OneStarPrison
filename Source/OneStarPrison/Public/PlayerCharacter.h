// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ONESTARPRISON_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float TurnRateGamepad;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, Replicated)
		bool CanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* ThrowCameraPos;

	//Interact with object/button press/hold
	void Interact();
	void StopInteract();
	UPROPERTY(VisibleAnywhere, Replicated)
	bool CanInteract = false;
	UPROPERTY(BlueprintReadWrite,Replicated)
	bool IsInteracting = false;

	UFUNCTION(Server, Reliable)
		void RPCInteract();
	UFUNCTION(Server, Reliable)
		void RPCStopInteract();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool IsGrabbing = false;


	//Grab Pickupable items and drop
	void PickupAndDrop();
	UFUNCTION(Server, Reliable)
		void ServerRPCPickupAndDrop();
	UFUNCTION(NetMulticast, Unreliable)
		void ClientRPCPickupAndDrop(APickupable* _Pickup);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool IsPickingUp = false;

	//Throw Picked up item
	void Throw();
	UFUNCTION(Server, Reliable)
		void ServerRPCThrow();
	UFUNCTION(NetMulticast, Reliable)
		void ClientRPCThrow();
	UFUNCTION(Client, Unreliable)
		void ClientShowThrowWidget();
	float cacheDeltaTime = 0;

	UPROPERTY()
	FRotator rot = FRotator::ZeroRotator;

	//Check if Throw Key is being held down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool IsHoldingDownThrow = false;

	//Current Throw Power
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ThrowPowerScale = 0;
	//Maximum Throw Power
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxThrowPower = 150.0f;
	UPROPERTY(EditAnywhere)
		float ThrowSpeed = 1.0f;
	//Current Player Index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int PlayerIndex = 0;

	UPROPERTY(BlueprintReadWrite)
		FVector cacheVelocity;
	//Throw Rendering
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USplineComponent* SplineComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<class USplineMeshComponent*> SplineComponentArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SplineMesh;
	UPROPERTY(EditAnywhere)
		class UMaterialInstance* SplineMeshMaterial;

	//HUD Class to add to viewport
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> ThrowWidgetClass;

	UPROPERTY(Replicated)
		class UUserWidget* CurrentThrowWidget;

	//Pickup Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> PickupWidgetClass;

	UPROPERTY(Replicated)
		class UUserWidget* CurrentPickupWidget;

	//Interact Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> InteractWidgetClass;

	UPROPERTY(Replicated)
		class UUserWidget* CurrentInteractWidget;

	//Show Pickup Widget
	UFUNCTION(Server, Reliable)
		void ServerCheckPickup();
	UFUNCTION(Client, Reliable)
		void CheckPickup();

	//Show Interact Widget
	UFUNCTION(Server, Reliable)
		void ServerCheckInteract();
	UFUNCTION(Client, Reliable)
		void CheckInteract();

	//Current Pickedup Item
	UPROPERTY(VisibleAnywhere, Replicated)
		class APickupable* PickedUpItem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector RespawnCheckpoint = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DeathTimer = 1.0f;

	float DeathTimerCounter = 0.0f;

	//Checks if dead then respawn player
	void CheckDeath(float _DeltaTime);
};
