// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveStepsButton.h"
#include "PlayerCharacter.h"
#include "InteractiveSteps.h"
#include "InteractiveStepsManager.h"

#include "Components/BoxComponent.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AInteractiveStepsButton::AInteractiveStepsButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveStepsButton::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractiveStepsButton::OnOverlapEnd);

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	MovableMesh->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AInteractiveStepsButton::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractiveStepsButton::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveStepsButton, LinkedSteps);
	DOREPLIFETIME(AInteractiveStepsButton, StepsManager);
}

// Called every frame
void AInteractiveStepsButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LinkedSteps.Num() > 0)
	{
		if (StepsManager)
		{
			if (OverlappingPlayer != nullptr)
			{
				if (OverlappingPlayer->IsInteracting)
				{
					MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime));
					StepsManager->IsStepOpen = true;
					StepsManager->SetOpenStep(LinkedSteps);
					StepsManager->OpenDoor(DeltaTime);
				}
				else
				{
					MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
					StepsManager->IsStepOpen = false;
					//StepsManager->SetOpenStep(LinkedSteps);
				}
			}
			else
			{
				MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
				//StepsManager->IsStepOpen = false;
				//StepsManager->SetOpenStep(LinkedSteps);
			}
		}

	}
}

void AInteractiveStepsButton::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				OverlappingPlayer = playerActor;
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, playerActor->GetName());
				OverlappingPlayer->CanInteract = true;
			}
		}
	}
}

void AInteractiveStepsButton::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		if (playerActor)
		{
			if (OverlappingPlayer != nullptr)
			{
				if (playerActor == OverlappingPlayer)
				{
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->IsInteracting = false;
					OverlappingPlayer = nullptr;
				}
			}
		}
	}
}
