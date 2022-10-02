#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/NPC.h"
#include "Task_EnablePatrol.generated.h"

UCLASS()
class ONESTARPRISON_API UTask_EnablePatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UTask_EnablePatrol(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
