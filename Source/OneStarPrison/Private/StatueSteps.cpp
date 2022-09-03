// Fill out your copyright notice in the Description page of Project Settings.


#include "StatueSteps.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "StatueManager.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AStatueSteps::AStatueSteps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	OnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OnMesh"));
	OffMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OffMesh"));

	OnMesh->SetupAttachment(RootComponent);
	OffMesh->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 100));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AStatueSteps::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AStatueSteps::BeginPlay()
{
	Super::BeginPlay();
	OnMesh->SetVisibility(false);
	OnMesh->SetVisibility(false);
}

void AStatueSteps::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStatueSteps, IsOn);
	DOREPLIFETIME(AStatueSteps, IsChosen);
	DOREPLIFETIME(AStatueSteps, Manager);
}

// Called every frame
void AStatueSteps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOn)
	{
		if (IsChosen)
		{
			OnMesh->SetVisibility(true);

		}
		else
		{
			OffMesh->SetVisibility(true);
		}
	}
	else
	{
		OnMesh->SetVisibility(false);
		OffMesh->SetVisibility(false);
	}
}

void AStatueSteps::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Manager)
	{
		if (Manager->PuzzleCompleted)
		{
			return;
		}
	}

	if (IsOn)
	{
		return;
	}

	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping player is a player
		if (playerActor)
		{
			//Set the overlapping player to this player 
			OverlappingPlayer = playerActor;

			if (!IsOn)
			{
				IsOn = true;
				if (IsChosen)
				{
					OnMesh->SetVisibility(true);
					if (CorrectSound)
					{
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(), CorrectSound, GetActorLocation());
					}
				}
				else
				{
					OffMesh->SetVisibility(true);
					if (FailSound)
					{
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FailSound, GetActorLocation());
					}
				}

			}
		}
	}
}

