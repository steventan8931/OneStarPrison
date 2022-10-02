#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/NPC.h"
#include "Task_StopChasingLocation.generated.h"

UCLASS()
class ONESTARPRISON_API UTask_StopChasingLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UTask_StopChasingLocation(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
