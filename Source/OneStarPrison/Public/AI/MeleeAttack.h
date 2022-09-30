#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/NPC.h"
#include "MeleeAttack.generated.h"

UCLASS()
class ONESTARPRISON_API UMeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UMeleeAttack(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	bool MontageHasFinished(ANPC* const NPC);
};
