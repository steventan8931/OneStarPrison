#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/NPC.h"
#include "Task_EndHearGuarding.generated.h"

UCLASS()
class ONESTARPRISON_API UTask_EndHearGuarding : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UTask_EndHearGuarding(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
