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
	Insert   UMETA(DisplayName = "Insert"),
	RowLeft   UMETA(DisplayName = "RowL"),
	RowRight   UMETA(DisplayName = "RowR"),
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

	//When the player presses the interact button on the server
	UFUNCTION(Client, Reliable)
		void Interact();
	UFUNCTION(Server, Reliable)
		void ServerInteract();
	//When the player releases the interact button on the server
	UFUNCTION(Client, Reliable)
		void StopInteract();
	UFUNCTION(Server, Reliable)
		void ServerStopInteract();

	//Interact UI
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

	//Pick Up UI
	//Have the server to check for the pick up
	UFUNCTION(Server, Reliable)
		void ServerCheckPickup();
	//Check whether to show pick up prompt on the client
	UFUNCTION(Client, Reliable)
		void CheckPickup();

	//Throw Picked up item
	UFUNCTION(Server, Reliable)
		void Throw();
	//Throws item from the player
	UFUNCTION(NetMulticast, Reliable)
		void ClientRPCThrow();
	//Shows throw UI on throwing player
	UFUNCTION(Client, Unreliable)
		void ClientShowThrowWidget();

	//caches the throw velocity from the predicted path to the actual path
	FVector cacheVelocity;

	//Throw Projectile Path Rendering
	UPROPERTY(VisibleAnywhere)
		class USplineComponent* SplineComponent;
	UPROPERTY(VisibleAnywhere)
		TArray<class USplineMeshComponent*> SplineComponentArray;
	UPROPERTY(EditAnywhere)
		UStaticMesh* SplineMesh;
	UPROPERTY(EditAnywhere)
		class UMaterialInstance* SplineMeshMaterial;

	//Renders the projectile path of the throwing item
	void ShowProjectilePath(float _DeltaTime);

	UFUNCTION(Server, Reliable)
		void ShowProjectileEnd(FVector _Location);

	//The scale of normal speed vs crouch speed
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float CrouchSpeedScale = 0.25f;
	//Makes the player crouch
	UFUNCTION(NetMulticast, Reliable)
		void StartCrouching();
	//On the server
	UFUNCTION(Server, Reliable)
		void ServerStartCrouching();
	//Makes the player uncrouch
	UFUNCTION(NetMulticast, Reliable)
		void StopCrouching();
	//On the server
	UFUNCTION(Server, Reliable)
		void ServerStopCrouching();

	//The speed of the player when climbing
	UPROPERTY(EditAnywhere, Replicated)
		float ClimbSpeed = 50.0f;
	//Updates the player variables for when they are/not climbing
	UFUNCTION(NetMulticast, Reliable)
		void CheckClimbing();

	//Checks if dead then respawn player
	void CheckDeath(float _DeltaTime);

	UPROPERTY(EditAnywhere, Replicated)
		TSubclassOf<class AActor> ThrowEndActorType;

	UPROPERTY(Replicated)
		AActor* cacheThrowEndActor;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//Checks whether the player can move or not (used with holing levers)
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		bool CanMove = true;
	
	//Interaction
	//Interact Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TEnumAsByte<EInteractType> InteractType;
	//Whetther the player is able to interact (Interact prompt shows up)
	UPROPERTY(Replicated)
	bool CanInteract = false;
	//Whether the player is interacting (holding down / clicked interact button)
	UPROPERTY(BlueprintReadWrite,Replicated)
	bool IsInteracting = false;

	//Pickup
	//Checks if the player is picking up (for the animation)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool IsPickingUp = false;

	//Throwing
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
		FVector ThrowCamOffset = FVector(0.0f, 50.0f, 50.0f);
	UPROPERTY(EditAnywhere)
		float ThrowCamSpeed = 5.0f;

	//Punch Interact timer
	UPROPERTY(BlueprintReadWrite, Replicated)
		float PunchTimer = 0.0f;
	//Punch Interact delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float PunchDelay = 0.5f;

	//Current Pickedup Item
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite)
		class APickupable* PickedUpItem = nullptr;

	//Crouching/Sneaking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool IsCrouching = false;

	//Pushing
	//Whether the player is in the box collision and able to push or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool CanPush = false;
	//Whether the player is pushing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool IsPushing = false;

	//Climbing
	//Checks whether the player is climbing
	UPROPERTY(BlueprintReadWrite, Replicated)
		bool IsClimbing = false;

	//Death
	//The location the player is respawn to 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector RespawnCheckpoint = FVector(0, 0, 0);
	//Check whether the player is dead or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDead = false;
	//Used by the death walls to check it has been hit by both walls
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		int HitByWallCount = 0;
	//Delay before the player is respawned after dying
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DeathTimer = 1.0f;
	//Counter for the death timer
	float DeathTimerCounter = 0.0f;

	//Cached armlength for when it will be changed for the maze
	UPROPERTY(BlueprintReadWrite)
		float cacheArmLength = 0.0f;

	//Returns whether the player is holding an heavy item
	bool CheckHeavyItem();
	//Checks whether the player is holding an heavy item
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool IsHoldingHeavyItem = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsInCell = false;
};
