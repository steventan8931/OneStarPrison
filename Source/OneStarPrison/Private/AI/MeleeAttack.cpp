// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MeleeAttack.h"
#include "AIController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

UMeleeAttack::UMeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI controller and its NPC
	if (AAIController* const Cont = OwnerComp.GetAIOwner())
	{
		// test to see if the NPC class supports the ICombatInterface interface
		if (ANPC* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			// if the montage has finished playing, play it again - stops "jittering"
			if (MontageHasFinished(NPC))
			{
				// execute the melee attack
				NPC->MeleeAttack();
			}
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

bool UMeleeAttack::MontageHasFinished(ANPC* const NPC)
{
	return NPC->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(NPC->GetMontage());
}
