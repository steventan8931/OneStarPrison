// Fill out your copyright notice in the Description page of Project Settings.


#include "StatueSteps.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AStatueSteps::AStatueSteps()
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

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AStatueSteps::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AStatueSteps::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AStatueSteps::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStatueSteps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		 
}

void AStatueSteps::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		//If it is meant to be interacted by players
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			//If the overlapping player is a player
			if (playerActor)
			{
				//Set the overlapping player to this player and allow them to interact
				OverlappingPlayer = playerActor;
				OverlappingPlayer->CanInteract = true;
				//Change the players interact type to lever pulling
				OverlappingPlayer->InteractType = EInteractType::LeverPull;

			}
		}
	}
}

void AStatueSteps::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If the overlapping player exists
			if (OverlappingPlayer)
			{
				//If this overlapping player is the overlapping player
				if (playerActor == OverlappingPlayer)
				{
					//Remove the players ability to interact with the lever
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer = nullptr;
				}
			}
		}

	}
}
