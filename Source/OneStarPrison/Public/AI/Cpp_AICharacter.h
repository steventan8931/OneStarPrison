// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundBase.h"
#include "AI/Cpp_AI_CharacterBase.h"
#include "Cpp_AICharacter.generated.h"

UCLASS(config=Game)
class ACpp_AICharacter : public ACpp_AI_CharacterBase
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ACpp_AICharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;
	
    void BeginPlay() override;

	// getters and setters for health etc.
    float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float const NewHealth);

	void Tick(float const DeltaTime) override;

protected:

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

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	class UWidgetComponent* WidgetComponent;
	float const MaxHealth = 100.0f;
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* DistractionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Montage;

	void OnExitGame();

	class UAIPerceptionStimuliSourceComponent* Stimulus;

	void SetupStimulus();

	void OnAttack();
	void OnDistract();

	UFUNCTION()
		void OnAttackOverlapBegin(
			UPrimitiveComponent* const OverlappedComponent,
			AActor* const OtherActor,
			UPrimitiveComponent* OtherComponent,
			int const OtherBodyIndex,
			bool const FromSweep,
			FHitResult const& SweepResult);

	UFUNCTION()
		void OnAttackOverlapEnd(
			UPrimitiveComponent* const OverlappedComponent,
			AActor* const OtherActor,
			UPrimitiveComponent* OtherComponent,
			int const OtherBodyIndex);

};

