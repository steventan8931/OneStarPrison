// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Cpp_AI_CharacterBase.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACpp_AI_CharacterBase::ACpp_AI_CharacterBase() : 
	RightFistCollisionBox(CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox")))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (RightFistCollisionBox)
	{
		FVector const Extent(8.0f);
		RightFistCollisionBox->SetBoxExtent(Extent, false);
		RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	}

}

// Called when the game starts or when spawned
void ACpp_AI_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (RightFistCollisionBox)
	{
		FAttachmentTransformRules const Rules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false);
		RightFistCollisionBox->AttachToComponent(GetMesh(), Rules, "hand_r_socket");
		RightFistCollisionBox->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));
	}
}

// Called every frame
void ACpp_AI_CharacterBase::Tick(float const DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACpp_AI_CharacterBase::AttackStart()
{
	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void ACpp_AI_CharacterBase::AttackEnd()
{
	RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);
}

