// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawbridgeTrigger.h"
#include "DrawbridgePlatform.h"
#include "Components/BoxComponent.h"
#include "Pickupable.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
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

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	MovableMesh->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ADrawbridgeTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADrawbridgeTrigger::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADrawbridgeTrigger, HandleOpenRotation);
	DOREPLIFETIME(ADrawbridgeTrigger, HandleClosedRotation);
}


// Called every frame
void ADrawbridgeTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Platform)
	{
		if (!Platform->IsOpen)
		{
			if (OverlappingPlayer != nullptr)
			{
				if (OverlappingPlayer->IsInteracting)
				{
					Platform->IsOpen = true;
					if (OpenSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
					}
					MovableMesh->SetRelativeRotation(HandleOpenRotation);
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->IsInteracting = false;
				}
			}
		}
		else
		{
			MovableMesh->SetRelativeRotation(HandleClosedRotation);
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
					if (OpenSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
					}
				}

			}
		}
		else
		{
			if (OverlappingPlayer == nullptr)
			{
				if (Platform)
				{
					if (!Platform->IsOpen)
					{
						APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

						if (playerActor)
						{
							OverlappingPlayer = playerActor;
							//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, playerActor->GetName());
							OverlappingPlayer->CanInteract = true;
							OverlappingPlayer->InteractType = EInteractType::LeverPull;

						}
					}
				}
			}
		}

	}
}

void ADrawbridgeTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
					OverlappingPlayer = nullptr;
				}
			}
		}

	}
}
