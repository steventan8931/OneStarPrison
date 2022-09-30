// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPath.h"
#include "Animation/AnimMontage.h"
#include "AI/Cpp_AI_CharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/PrimitiveComponent.h"
#include "NPC.generated.h"

UCLASS()
class ONESTARPRISON_API ANPC : public ACpp_AI_CharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();
		
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APatrolPath* GetPatrolPath();
	
	
	UFUNCTION(NetMulticast, Reliable)
	    void MeleeAttack();
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void AlertStart();
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void AlertEnd();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
		void SetCameraParameters(UMaterialInstanceDynamic* Mat);

	UFUNCTION()
		void MulticastElim(bool bPlayerLeftGame);
	UAnimMontage* GetMontage() const;

	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float const NewHealth);
	UBehaviorTree* GetBehaviorTree() const;
	void SetPatrolPath(APatrolPath* const Path);

	float GetSightRadius();
	float GetLoseSightRadius();
	float GetPeripheralVisionAngleDegrees();
	float GetHearingRange();
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> ItemInterested;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		bool ShowVision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		bool FearLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		bool IsAlert ;





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	class UWidgetComponent* WidgetComponent;
	float const MaxHealth = 100.0f;
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess= "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float SightRadius = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float LoseSightRadius = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float PeripheralVisionAngleDegrees = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float HearingRange = 3000.0f;

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
