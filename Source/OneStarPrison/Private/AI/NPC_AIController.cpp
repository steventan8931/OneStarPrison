// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPC_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/blackboard_keys.h"
#include "AI/ai_tags.h"
#include "AI/NPC.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void ANPC_AIController::OnPossess(APawn* const CharacterPawn)
{
	Super::OnPossess(CharacterPawn);
	if (auto const NPC = Cast<ANPC>(CharacterPawn))
	{
		NPCobj = NPC;
		if (UBehaviorTree* const Tree = NPC->GetBehaviorTree())
		{
			UseBlackboard(Tree->BlackboardAsset, BlackboardComponent);
			RunBehaviorTree(Tree);
		}
		if (NPC->GetPatrolPath())
		{
			BlackboardComponent->SetValueAsBool(BBKeys::GetPatrolPath, true);
			
		}
		if (NPC->GetWalkSpeed())
		{
			BlackboardComponent->SetValueAsFloat(BBKeys::WalkSpeed, NPC->GetWalkSpeed());

		}		
		if (NPC->GetChaseSpeed())
		{
			BlackboardComponent->SetValueAsFloat(BBKeys::ChaseSpeed, NPC->GetChaseSpeed());

		}
			ItemInterested = NPC->ItemInterested;
			 CanHearPlayer = NPC->CanHearPlayer;
			 CanHearRock = NPC->CanHearRock;
			 CanHearBone = NPC->CanHearBone;
		
	}
}

void ANPC_AIController::OnUpdated(TArray<AActor*> const& UpdatedActors)
{    

	//BlackboardComponent->SetValueAsBool(BBKeys::CanSeePlayer, false);
	
	for (size_t x = 0; x < UpdatedActors.Num(); ++x)
	{
		FActorPerceptionBlueprintInfo Info;
		GetPerceptionComponent()->GetActorsPerception(UpdatedActors[x], Info);

		for (size_t k = 0; k < Info.LastSensedStimuli.Num(); ++k)
		{
			FAIStimulus const Stim = Info.LastSensedStimuli[k];
			
			if (BlackboardComponent && Stim.Type.Name == "Default__AISense_Hearing"){
				if(CanHearPlayer){
				if (BlackboardComponent && Stim.Tag == tags::PlayerTag)
				{
					APlayerCharacter* player = Cast<APlayerCharacter>(UpdatedActors[x]);
					if (player)
					{
						if (!(player->IsInCell))
						{

							BlackboardComponent->SetValueAsBool(BBKeys::IsInHearCaution, Stim.WasSuccessfullySensed());
							BlackboardComponent->SetValueAsVector(BBKeys::TargetLocation, Stim.StimulusLocation);
							BlackboardComponent->SetValueAsObject(BBKeys::TargetLocationActor, UpdatedActors[x]);
						}
					}
					
				}
				}
				if (CanHearRock) {
					if (BlackboardComponent && Stim.Tag == tags::RockTag)
					{
						BlackboardComponent->SetValueAsBool(BBKeys::IsInHearCaution, Stim.WasSuccessfullySensed());
						BlackboardComponent->SetValueAsVector(BBKeys::TargetLocation, Stim.StimulusLocation);
						BlackboardComponent->SetValueAsObject(BBKeys::TargetLocationActor, UpdatedActors[x]);
						if (NPCobj != nullptr)
						{
							NPCobj->PlayCautionAudio();
						}
					}
				}
				if (CanHearBone) {
					if (BlackboardComponent && Stim.Tag == tags::BoneTag)
					{
						BlackboardComponent->SetValueAsBool(BBKeys::IsInHearCaution, Stim.WasSuccessfullySensed());
						BlackboardComponent->SetValueAsVector(BBKeys::TargetLocation, Stim.StimulusLocation);
						BlackboardComponent->SetValueAsObject(BBKeys::TargetLocationActor, UpdatedActors[x]);
						if (NPCobj != nullptr)
						{
							NPCobj->PlayCautionAudio();
						}

					}
				}
			}
			
				else if (BlackboardComponent && Stim.Type.Name == "Default__AISense_Sight")
				{
					if (ItemInterested == UpdatedActors[x]->GetClass()) 
					{
						APlayerCharacter* player = Cast<APlayerCharacter>(UpdatedActors[x]);
						if (player)
						{
							if (!(player->IsInCell))
							{
								BlackboardComponent->SetValueAsBool(BBKeys::IsInvestigating, Stim.WasSuccessfullySensed());
								BlackboardComponent->SetValueAsBool(BBKeys::CanSeePlayer, Stim.WasSuccessfullySensed());
								BlackboardComponent->SetValueAsObject(BBKeys::ItemInteresting, UpdatedActors[x]);
							}
							
						}
						else {
							BlackboardComponent->SetValueAsBool(BBKeys::IsInvestigating, Stim.WasSuccessfullySensed());
							BlackboardComponent->SetValueAsBool(BBKeys::CanSeePlayer, Stim.WasSuccessfullySensed());
							BlackboardComponent->SetValueAsObject(BBKeys::ItemInteresting, UpdatedActors[x]);
						}
						

				    }
				
			    }
			

			

		}
	}
}
void  ANPC_AIController::SetAlert(bool IsAlert)
{
	BlackboardComponent->SetValueAsBool(BBKeys::IsAlert, IsAlert);

}

void ANPC_AIController::SetupPerceptionSystem()
{
	// create and initialise sight configuration object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		// add sight configuration component to perception component
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

	// create and initialise hearing config object
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing config"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 3000.0f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		// add sight configuration component to perception component
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ANPC_AIController::OnUpdated);
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
	
}


class AActor* ANPC_AIController::GetInterestedItem()
{
	
	if (BlackboardComponent)
	{
		return Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(BBKeys::ItemInteresting));
	}

	return NULL;
}



