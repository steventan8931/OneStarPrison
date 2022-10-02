// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task_Wait.h"
#include "AIController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Animation/AnimInstance.h"
#include <AI\blackboard_keys.h>

UTask_Wait::UTask_Wait(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Wait";
	WaitTime = 5.0f;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}



EBTNodeResult::Type UTask_Wait::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKeys::IsHearGuarding, false);
	return EBTNodeResult::Aborted;
}



EBTNodeResult::Type UTask_Wait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTWaitTaskMemory* MyMemory = CastInstanceNodeMemory<FBTWaitTaskMemory>(NodeMemory);
	MyMemory->RemainingWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));

	return EBTNodeResult::InProgress;
}

void UTask_Wait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTWaitTaskMemory* MyMemory = CastInstanceNodeMemory<FBTWaitTaskMemory>(NodeMemory);
	MyMemory->RemainingWaitTime -= DeltaSeconds;

	if (MyMemory->RemainingWaitTime <= 0.0f)
	{
		// continue execution from this node
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

FString UTask_Wait::GetStaticDescription() const
{
	if (FMath::IsNearlyZero(RandomDeviation))
	{
		return FString::Printf(TEXT("%s: %.1fs"), *Super::GetStaticDescription(), WaitTime);
	}
	else
	{
		return FString::Printf(TEXT("%s: %.1f+-%.1fs"), *Super::GetStaticDescription(), WaitTime, RandomDeviation);
	}
}

void UTask_Wait::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	FBTWaitTaskMemory* MyMemory = CastInstanceNodeMemory<FBTWaitTaskMemory>(NodeMemory);
	if (MyMemory->RemainingWaitTime)
	{
		Values.Add(FString::Printf(TEXT("remaining: %ss"), *FString::SanitizeFloat(MyMemory->RemainingWaitTime)));
	}
}

uint16 UTask_Wait::GetInstanceMemorySize() const
{
	return sizeof(FBTWaitTaskMemory);
}
