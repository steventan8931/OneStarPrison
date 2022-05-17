// Fill out your copyright notice in the Description page of Project Settings.


#include "CrankliftTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "CrankliftPlatform.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ACrankliftTrigger::ACrankliftTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACrankliftTrigger::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ACrankliftTrigger::OnOverlapEnd);

	//CurrentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
}

// Called when the game starts or when spawned
void ACrankliftTrigger::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(200, 200, 200), FColor::Purple, true);
}

// Called every frame
void ACrankliftTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Platform)
	{
		if (OverlappingPlayer != nullptr)
		{
			if (OverlappingPlayer->IsInteracting)
			{
				IsMovingUp = true;
			}
			else
			{
				IsMovingUp = false;
			}
		}

		if (IsMovingUp)
		{
			if (Platform->GetActorLocation().Z <= MaxHeight)
			{
				Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z + DeltaTime * MoveSpeed));
			}
		}
		else
		{
			if (Platform->GetActorLocation().Z >= MinHeight)
			{
				Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z - DeltaTime * MoveSpeed));
			}
		}
	}




}

void ACrankliftTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				OverlappingPlayer = playerActor;
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Can Interact"));
				OverlappingPlayer->CanInteract = true;
			}
		}


	}
}

void ACrankliftTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer != nullptr)
		{
			IsMovingUp = false;
			OverlappingPlayer = nullptr;
		}

	}
}