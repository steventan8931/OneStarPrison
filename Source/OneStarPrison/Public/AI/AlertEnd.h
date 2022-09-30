#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/NPC.h"
#include "AlertEnd.generated.h"

UCLASS()
class ONESTARPRISON_API UAlertEnd : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UAlertEnd(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
