// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IsPlayerInMeleeRange.h"
#include "AI/NPC_AIController.h"
#include "AI/NPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AI/blackboard_keys.h"
#include <cmath>

UIsPlayerInMeleeRange::UIsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player in Melee Range");
}

void UIsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// get NPC
	if (ANPC_AIController* const Cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			// get player character
			if (AActor* const Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BBKeys::ItemInteresting)))
			{
				float const Distance = NPC->GetDistanceTo(Player);
				// write true or false depending on whether the player is within melee_range
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Distance <= MeleeRange);
			}
			else
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
		}
		else
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
	}
	else
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
}
