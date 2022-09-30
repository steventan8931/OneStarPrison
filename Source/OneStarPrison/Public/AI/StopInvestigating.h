#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "StopInvestigating.generated.h"

UCLASS()
class ONESTARPRISON_API UStopInvestigating : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UStopInvestigating(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
