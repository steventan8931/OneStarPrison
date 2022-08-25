// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum EInteractType
{
	LeverPull     UMETA(DisplayName = "LeverPull"),
	Punch      UMETA(DisplayName = "Punch"),
	//Grab   UMETA(DisplayName = "Footwear"),
};

UCLASS()
class ONESTARPRISON_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
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

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Throw Widget Blueprint
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> ThrowWidgetClass;
	//Pickup Widget Blueprint
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> PickupWidgetClass;
	//Interact Widget Blueprint
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> InteractWidgetClass;

	//Cache of Current Widgets
	UPROPERTY(Replicated)
		class UUserWidget* CurrentThrowWidget;
	UPROPERTY(Replicated)
		class UUserWidget* CurrentPickupWidget;
	UPROPERTY(Replicated)
		class UUserWidget* CurrentInteractWidget;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//Checks whether the player can move or not (used with holing levers)
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		bool CanMove = true;
	
	//Cached armlength for when it will be changed for the maze
	UPROPERTY(BlueprintReadWrite)
	float cacheArmLength = 0.0f;

	//When the player presses the interact button
	void Interact();
	//Call function on the server
	UFUNCTION(Server, Reliable)
		void RPCInteract();
	//When the player releases the interact button
	void StopInteract();
	//Call function on the server
	UFUNCTION(Server, Reliable)
		void RPCStopInteract();
	//Whetther the player is able to interact (Interact prompt shows up)
	UPROPERTY(Replicated)
	bool CanInteract = false;
	//Whether the player is interacting (holding down / clicked interact button)
	UPROPERTY(BlueprintReadWrite,Replicated)
	bool IsInteracting = false;

	//Have the server to check for the interact
	UFUNCTION(Server, Reliable)
		void ServerCheckInteract();
	//Check whether to show interact prompt on the client
	UFUNCTION(Client, Reliable)
		void CheckInteract();

	//When the player presses the pick up button
	void PickupAndDrop();
	//Call Pick up on the server
	UFUNCTION(Server, Reliable)
		void ServerRPCPickupAndDrop();
	//Call Pick up on the client
	UFUNCTION(NetMulticast, Unreliable)
		void ClientRPCPickupAndDrop(APickupable* _Pickup);
	//Checks if the player is picking up (for the animation)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool IsPickingUp = false;

	//Have the server to check for the pick up
	UFUNCTION(Server, Reliable)
		void ServerCheckPickup();
	//Check whether to show pick up prompt on the client
	UFUNCTION(Client, Reliable)
		void CheckPickup();

	//Throw Picked up item
	void Throw();
	UFUNCTION(Server, Reliable)
		void ServerRPCThrow();
	UFUNCTION(NetMulticast, Reliable)
		void ClientRPCThrow();
	UFUNCTION(Client, Unreliable)
		void ClientShowThrowWidget();



	//Check if Throw Key is being held down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool IsHoldingDownThrow = false;

	//Current Throw Power
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ThrowPowerScale = 0;
	//Maximum Throw Power
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxThrowPower = 150.0f;

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

	//Punch Interact delay
	UPROPERTY(BlueprintReadWrite, Replicated)
		float PunchTimer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float PunchDelay = 0.5f;

	//Current Pickedup Item
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		class APickupable* PickedUpItem = nullptr;

	//Death
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector RespawnCheckpoint = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		int HitByWallCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DeathTimer = 1.0f;
	float DeathTimerCounter = 0.0f;
	//Checks if dead then respawn player
	void CheckDeath(float _DeltaTime);

	//Pushing
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		bool CanPush = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		bool IsPushing = false;


	//Crouching/Sneaking
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
		bool IsCrouching = false;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float CrouchSpeedScale = 0.25f;
	UFUNCTION(Server, Reliable)
	void StartCrouching();
	UFUNCTION(Server, Reliable)
	void StopCrouching();

	//Climbing
	UPROPERTY(BlueprintReadWrite, Replicated)
		bool IsClimbing = false;
	UPROPERTY(EditAnywhere, Replicated)
		float ClimbSpeed = 50.0f;
	UFUNCTION(NetMulticast, Reliable)
	void CheckClimbing();

	//Posssesion
	UFUNCTION(Server, Reliable)
	void SetVeloctiy(FVector _Velocity);
		FTransform GetCameraTransform();

		//Interact Type
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TEnumAsByte<EInteractType> InteractType;

};
