// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/StopInvestigating.h"
#include "AI/NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/blackboard_keys.h"
#include "AI/NPC.h"
UStopInvestigating::UStopInvestigating(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Stop Investigating");
}

EBTNodeResult::Type UStopInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get blackboard and set IsInvestigating key to false to stop NPC from investigating
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);

	if (AAIController* const Cont = OwnerComp.GetAIOwner())
	{
		// test to see if the NPC class supports the ICombatInterface interface
		if (ANPC* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			NPC->SetCautionVisibility(false);
		}
	}

	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

}
