// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IncrementPathIndex.h"
#include "AI/NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NPC.h"
#include "AI/blackboard_keys.h"
#include <cmath>

UIncrementPathIndex::UIncrementPathIndex(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get the AI controller
	if (ANPC_AIController* const Controller = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* const NPC = Cast<ANPC>(Controller->GetPawn()))
		{
			int const NoOfPoints = NPC->GetPatrolPath()->Num();
			int const MinIndex = 0;
			int const MaxIndex = NoOfPoints - 1;

			// get and set the black board index key
			int Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());
			if (BiDirectional)
			{
				if (Index >= MaxIndex && Direction == EDirectionType::Forward)
				{
					Direction = EDirectionType::Reverse;
				}
				else if (Index == MinIndex && Direction == EDirectionType::Reverse)
				{
					Direction = EDirectionType::Forward;
				}
			}
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(),
				(Direction == EDirectionType::Forward ?
					std::abs(++Index) :
					std::abs(--Index)) % NoOfPoints);

			//finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return  EBTNodeResult::Failed;
}
