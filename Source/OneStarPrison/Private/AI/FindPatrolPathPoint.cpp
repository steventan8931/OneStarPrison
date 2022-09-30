// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FindPatrolPathPoint.h"
#include "AI/NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NPC.h"
#include "AI/blackboard_keys.h"

UFindPatrolPathPoint::UFindPatrolPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Patrol Path Point");
}

EBTNodeResult::Type UFindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get the AI controller for the patrolling NPC
	if (ANPC_AIController* const Controller = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		// get the current patrol path index from the blackboard
		int const Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

		// use the index to get the current patrol path from the NPC's reference to the patrol path
		if (ANPC* const NPC = Cast<ANPC>(Controller->GetPawn()))
		{
			FTransform const PointTransform = NPC->GetPatrolPath()->GetPatrolPoint(Index);
			
			FVector const Point = PointTransform.GetLocation();
			FRotator const PointRotation = PointTransform.Rotator();

			// transform this point to a global position using its parent
			FVector const GlobalPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

			// write the current global path point to the blackboard
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);
			OwnerComp.GetBlackboardComponent()->SetValueAsRotator(BBKeys::PatrolRotation, PointRotation);

			// finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
