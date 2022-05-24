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

	//Interact with object/button press/hold
	void Interact();
	void StopInteract();
	UPROPERTY(VisibleAnywhere, Replicated)
	bool CanInteract = false;
	UPROPERTY(VisibleAnywhere, Replicated)
	bool IsInteracting = false;

	UFUNCTION(Server, Reliable)
		void RPCInteract();
	UFUNCTION(Server, Reliable)
		void RPCStopInteract();

	//Grab Pickupable items and drop
	void PickupAndDrop();
	UFUNCTION(Server, Reliable)
		void RPCPickupAndDrop();

	//Throw Picked up item
	void Throw();

	//Check if Throw Key is being held down
	bool IsHoldingDownThrow = false;

	//Current Throw Power
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ThrowPowerScale = 0;
	//Maximum Throw Power
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxThrowPower = 150.0f;
	//Current Player Index
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlayerIndex = 0;


	//HUD Class to add to viewport
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere)
		class UUserWidget* CurrentWidget;

	//Current Pickedup Item
	UPROPERTY(VisibleAnywhere, Replicated)
		class APickupable* PickedUpItem;
};
