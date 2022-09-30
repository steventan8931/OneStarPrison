#pragma once


#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackNotifyState.generated.h"

UCLASS()
class ONESTARPRISON_API UAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:	
	void NotifyBegin(USkeletalMeshComponent* const Mesh, UAnimSequenceBase* const Animation, float const TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* const Mesh, UAnimSequenceBase* const Animation) override;
};