// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AttackNotifyState.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI/Cpp_AI_CharacterBase.h"

void UAttackNotifyState::NotifyBegin(
	USkeletalMeshComponent* const Mesh, 
	UAnimSequenceBase* const Animation, 
	float const TotalDuration)
{
	if (Mesh && Mesh->GetOwner())
	{
		if (ACpp_AI_CharacterBase* const Character = Cast<ACpp_AI_CharacterBase>(Mesh->GetOwner()))
		{
			Character->AttackStart();
		}
	}
}

void UAttackNotifyState::NotifyEnd(
	USkeletalMeshComponent* const Mesh, 
	UAnimSequenceBase* const Animation)
{
	if (Mesh && Mesh->GetOwner())
	{
		if (ACpp_AI_CharacterBase* const Character = Cast<ACpp_AI_CharacterBase>(Mesh->GetOwner()))
		{
			Character->AttackEnd();
		}
	}
}
