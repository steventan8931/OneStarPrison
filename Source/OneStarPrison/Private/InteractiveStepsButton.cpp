// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveStepsButton.h"
#include "PlayerCharacter.h"
#include "InteractiveSteps.h"
#include "InteractiveStepsManager.h"

#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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
}

// Called when the game starts or when spawned
void AInteractiveStepsButton::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractiveStepsButton::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveStepsButton, CurrentWidget);
	DOREPLIFETIME(AInteractiveStepsButton, OverlappingPlayer);
}

// Called every frame
void AInteractiveStepsButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LinkedSteps.Num() > 0)
	{
		if (OverlappingPlayer != nullptr)
		{
			if (OverlappingPlayer->IsInteracting)
			{
				StepsManager->IsStepOpen = true;
				StepsManager->SetOpenStep(LinkedSteps);
				StepsManager->OpenDoor(DeltaTime);
			}
			else
			{
				StepsManager->IsStepOpen = false;
				StepsManager->SetOpenStep(LinkedSteps);
			}
		}
		else
		{
			StepsManager->IsStepOpen = false;
			StepsManager->SetOpenStep(LinkedSteps);
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
				ServerInteractPopUp();
			}
		}
	}
}

void AInteractiveStepsButton::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer != nullptr)
		{
			ServerRemoveWidget();

			OverlappingPlayer = nullptr;
		}

	}
}

void AInteractiveStepsButton::ServerInteractPopUp_Implementation()
{
	InteractPopUp();
}
void AInteractiveStepsButton::InteractPopUp_Implementation()
{
	if (HUDWidgetClass != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("WIDGET CLASS EXIST"));

		CurrentWidget = CreateWidget<UUserWidget>(OverlappingPlayer->GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToPlayerScreen();
		}
	}
}

void AInteractiveStepsButton::ServerRemoveWidget_Implementation()
{
	RemoveWidget();
}

void AInteractiveStepsButton::RemoveWidget_Implementation()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}
}