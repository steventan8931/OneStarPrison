// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ChangeSpeed.h"
#include "AI/NPC.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UChangeSpeed::UChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	if (auto const Cont = OwnerComp.GetAIOwner())
	{
		if (ANPC* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}

FString UChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the NPC speed");
}
