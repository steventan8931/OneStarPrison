// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPC.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "AI/Cpp_AICharacter.h"
#include <Kismet\GameplayStatics.h>
#include <AI\NPC_AIController.h>
#include <AI\blackboard_keys.h>

// Sets default values
ANPC::ANPC() :

	Health(MaxHealth)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;


}



void ANPC::MulticastElim(bool bPlayerLeftGame)
{
}

UAnimMontage* ANPC::GetMontage() const
{
	return Montage;
}

float ANPC::GetHealth() const
{
	return Health;
}

float ANPC::GetMaxHealth() const
{
	return MaxHealth;
}

void ANPC::SetHealth(float const NewHealth)
{
	Health = NewHealth;
	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("You killed the guard!!!"));
		
	}
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	// set material color of character
	UMaterialInstanceDynamic* const MaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue("BodyColor", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		GetMesh()->SetMaterial(0, MaterialInstance);
	}

	// attach delegates
	if (RightFistCollisionBox)
	{
		RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnAttackOverlapBegin);
		RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnAttackOverlapEnd);
	}
}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	return BehaviorTree;
}

void ANPC::SetPatrolPath(APatrolPath* const Path)
{
	PatrolPath = Path;
}

float ANPC::GetSightRadius() 
{
	return SightRadius;
}

float ANPC::GetLoseSightRadius() 
{
	return LoseSightRadius + SightRadius;
}

float ANPC::GetPeripheralVisionAngleDegrees() 
{
	return PeripheralVisionAngleDegrees;
}

float ANPC::GetHearingRange() 
{
	return HearingRange;
}

float ANPC::GetChaseSpeed()
{
	return ChaseSpeed;
}

float ANPC::GetWalkSpeed()
{
	return WalkSpeed;
}



void ANPC::OnAttackOverlapBegin(
	UPrimitiveComponent* const OverlappedComponent, 
	AActor* const OtherActor, 
	UPrimitiveComponent* OtherComponent, 
	int const OtherBodyIndex, 
	bool const FromSweep, 
	FHitResult const& SweepResult)
{
	
}

void ANPC::OnAttackOverlapEnd(
	UPrimitiveComponent* const OverlappedComponent, 
	AActor* const OtherActor, 
	UPrimitiveComponent* OtherComponent, 
	int const OtherBodyIndex)
{
}



// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APatrolPath* ANPC::GetPatrolPath()
{
	return PatrolPath;
}

void ANPC::AlertStart_Implementation()
{
	IsAlert = true;
	FVector Mylocation = GetActorLocation();
	UGameplayStatics::SpawnSoundAtLocation(this,SirenAudio, Mylocation);
}

void ANPC::AlertEnd_Implementation()
{
	IsAlert = false;
	
	FVector Mylocation = GetActorLocation();
	UGameplayStatics::SpawnSoundAtLocation(this, RecoveryAudio, Mylocation);
}




void ANPC::MeleeAttack_Implementation()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
		AController* MyController = GetController();
		ANPC_AIController* AI = Cast<ANPC_AIController>(MyController);
		
		if (AI)
		{
		AActor* AimActor = AI->GetInterestedItem();	
		if (AimActor) 
		    {
			UGameplayStatics::ApplyDamage(AimActor, 10.0f, MyController, NULL, NULL);
		    }

		}

	}
}

