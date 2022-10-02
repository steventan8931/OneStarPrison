#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/NPC.h"
#include "Task_DisablePatrol.generated.h"

UCLASS()
class ONESTARPRISON_API UTask_DisablePatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UTask_DisablePatrol(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
