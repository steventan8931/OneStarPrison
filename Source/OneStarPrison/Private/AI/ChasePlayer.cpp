// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ChasePlayer.h"
#include "AI/NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AI/blackboard_keys.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get TargetLocation from blackboard via AI controller
	if (ANPC_AIController* const Cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		FVector const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// move to the player's location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Cont, PlayerLocation);

		// finish with success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
