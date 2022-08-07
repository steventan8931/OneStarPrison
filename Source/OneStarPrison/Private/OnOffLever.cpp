// Fill out your copyright notice in the Description page of Project Settings.


#include "OnOffLever.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

#include "OnOffDoorLeverManager.h"

// Sets default values
AOnOffLever::AOnOffLever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(150, 150, 150));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AOnOffLever::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AOnOffLever::OnOverlapEnd);

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	MovableMesh->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AOnOffLever::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOnOffLever::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOnOffLever, HandleOpenRotation);
	DOREPLIFETIME(AOnOffLever, HandleClosedRotation);
	DOREPLIFETIME(AOnOffLever, IsOpen);


	DOREPLIFETIME(AOnOffLever, Manager);
	DOREPLIFETIME(AOnOffLever, IndexInManager);
}

// Called every frame
void AOnOffLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingPlayer != nullptr)
	{

		if (OpenTimer >= OpenTime)
		{
			OverlappingPlayer->CanInteract = true;
			OverlappingPlayer->InteractType = EInteractType::LeverPull;
		}

		if (OverlappingPlayer->IsInteracting)
		{
			if (IsOpen)
			{
				IsOpen = false;
				if (OpenSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
				}

				if (Manager)
				{
					for (int i = 0; i < Manager->OnOffLeverDoors.Num(); i++)
					{
						Manager->OnOffLeverDoors[IndexInManager].Doors[i]->PlaySound();
					}

				}

				OverlappingPlayer->IsInteracting = false;

				OverlappingPlayer->CanInteract = false;
				OpenTimer = 0.0f;
			}
			else
			{
				IsOpen = true;
				if (OpenSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
				}

				if (Manager)
				{
					for (int i = 0; i < Manager->OnOffLeverDoors.Num(); i++)
					{
						Manager->OnOffLeverDoors[IndexInManager].Doors[i]->PlaySound();
					}

				}

				OverlappingPlayer->IsInteracting = false;

				OverlappingPlayer->CanInteract = false;
				OpenTimer = 0.0f;
			}

		}
	}

	if (IsOpen)
	{
		OpenTimer += DeltaTime;
		MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime));
	}
	else
	{
		OpenTimer += DeltaTime;
		MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
	}

}

void AOnOffLever::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				OverlappingPlayer = playerActor;

			}
		}
	}
}

void AOnOffLever::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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