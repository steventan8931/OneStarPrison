// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawbridgeTrigger.h"
#include "DrawbridgePlatform.h"
#include "Components/BoxComponent.h"
#include "Pickupable.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADrawbridgeTrigger::ADrawbridgeTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("Trigger"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADrawbridgeTrigger::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ADrawbridgeTrigger::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ADrawbridgeTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrawbridgeTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Platform)
	{
		if (OverlappingPlayer != nullptr)
		{
			if (OverlappingPlayer->IsInteracting)
			{
				Platform->IsOpen = true;
				CurrentWidget->RemoveFromParent();
			}
		}
	}
}

void ADrawbridgeTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (!IsPlayerInteractable)
		{
			APickupable* pickupable = Cast<APickupable>(OtherActor);

			if (pickupable)
			{
				if (Platform)
				{
					Platform->IsOpen = true;
				}

			}
		}
		else
		{
			if (OverlappingPlayer == nullptr)
			{
				APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

				if (playerActor)
				{
					OverlappingPlayer = playerActor;
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, playerActor->GetName());
					OverlappingPlayer->CanInteract = true;
					InteractPopUp();
				}
			}
		}

	}
}

void ADrawbridgeTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("leffttt"));
	if (OtherActor && (OtherActor != this))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("leffttt"));
		if (OverlappingPlayer != nullptr)
		{


			if (CurrentWidget)
			{
				CurrentWidget->RemoveFromParent();
			}

			OverlappingPlayer = nullptr;
		}

	}
}

void ADrawbridgeTrigger::InteractPopUp()
{
	if (HUDWidgetClass != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("WIDGET CLASS EXIST"));

		CurrentWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), OverlappingPlayer->PlayerIndex), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToPlayerScreen();
		}
	}
}
