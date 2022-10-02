#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/NPC.h"
#include "Task_DogGoForFood.generated.h"

UCLASS()
class ONESTARPRISON_API UTask_DogGoForFood : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UTask_DogGoForFood(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
