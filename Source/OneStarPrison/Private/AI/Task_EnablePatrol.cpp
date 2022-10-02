// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task_EnablePatrol.h"
#include "AIController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Animation/AnimInstance.h"
#include <AI\blackboard_keys.h>

UTask_EnablePatrol::UTask_EnablePatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Enable Patrol");
}

EBTNodeResult::Type UTask_EnablePatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI controller and its NPC
	if (AAIController* const Cont = OwnerComp.GetAIOwner())
	{
		// test to see if the NPC class supports the ICombatInterface interface
		if (ANPC* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKeys::IsPatroling, true);
			
		}

		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

