#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/NPC.h"
#include "AlertStart.generated.h"

UCLASS()
class ONESTARPRISON_API UAlertStart : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UAlertStart(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
