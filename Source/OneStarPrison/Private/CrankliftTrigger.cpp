// Fill out your copyright notice in the Description page of Project Settings.


#include "CrankliftTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "CrankliftPlatform.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	MovableMesh->SetupAttachment(Mesh);

	HoldPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HoldPosition"));
	HoldPosition->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ACrankliftTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (MovingSound)
	{
		if (Platform)
		{
			AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, MovingSound, Platform->GetActorLocation());

			if (AudioComponent)
			{
				ServerPlaySound(true);
				AudioComponent->SetPaused(true);
			}

		}
	}

}

void ACrankliftTrigger::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACrankliftTrigger, HandleOpenRotation);
	DOREPLIFETIME(ACrankliftTrigger, HandleClosedRotation);
	DOREPLIFETIME(ACrankliftTrigger, AudioComponent);

	DOREPLIFETIME(ACrankliftTrigger, MaxHeight);
	DOREPLIFETIME(ACrankliftTrigger, MinHeight);
	DOREPLIFETIME(ACrankliftTrigger, IsMovingUp);
	DOREPLIFETIME(ACrankliftTrigger, Platform);

	DOREPLIFETIME(ACrankliftTrigger, HandleRotateSpeed);
	DOREPLIFETIME(ACrankliftTrigger, HoldPosition);
	DOREPLIFETIME(ACrankliftTrigger, HandleStartDelay);
	DOREPLIFETIME(ACrankliftTrigger, HandleStartTimer);
}

// Called every frame
void ACrankliftTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If there is a cranklift platform linked
	if (Platform)
	{
		if (OverlappingPlayer != nullptr)
		{
			//If Overlapping player is holding down the lever
			if (OverlappingPlayer->IsInteracting)
			{
				//Snap to location
				OverlappingPlayer->SetActorLocation(HoldPosition->GetComponentLocation());
				OverlappingPlayer->SetActorRotation(HoldPosition->GetComponentRotation());

				if (AudioComponent)
				{
					if (!AudioComponent->IsPlaying())
					{
						ServerPlaySound(false);
					}
				}

				//Move the platform up
				IsMovingUp = true;
			}
			else
			{

				//Move the platform down
				IsMovingUp = false;
			}
		}
		else
		{
			//Move the platform down if the player is not interacting
			IsMovingUp = false;
		}

		//If the platform is moving up
		if (IsMovingUp)
		{
			HandleStartTimer += DeltaTime;

			//If the platform has not reached its max hegiht
			if (Platform->GetActorLocation().Z <= MaxHeight)
			{
				//Move the handle to the open position over time
				if (HandleStartTimer >= HandleStartDelay)
				{
					MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime * HandleRotateSpeed));
				}

				//Keep moving the platform up over time
				Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z + DeltaTime * MoveSpeed));

				//If the platform is almost at its max height pause the audio
				if ((MaxHeight - Platform->GetActorLocation().Z) < 100)
				{
					ServerPlaySound(true);
				}
				else
				{
					ServerPlaySound(false);
				}
			}
			else
			{
				ServerPlaySound(true);
			}

		}
		else
		{
			HandleStartTimer = 0;

			//If the platform has not reached its minimum height
			if (Platform->GetActorLocation().Z >= MinHeight)
			{
				//Move the handle to the closed position over time
				MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
				//Keep moving the platform down over time
				Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z - DeltaTime * MoveSpeed));

				//If the platform is almost at its min height pause the audio
				if ((Platform->GetActorLocation().Z - MinHeight) < 100)
				{
					ServerPlaySound(true);
				}
				else
				{
					ServerPlaySound(false);

				}
			}
			else
			{
				ServerPlaySound(true);
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

			//If there is an overlapping actor is a player, allow them to interact
			if (playerActor)
			{
				OverlappingPlayer = playerActor;
				OverlappingPlayer->CanInteract = true;
				//Change the players interact type to punching
				OverlappingPlayer->InteractType = EInteractType::LeverPull;
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
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
			if (playerActor)
			{
				//If there is an overlapping player and if they player is leaving actor, make them unable to interact and set overlapping to nullptr
				if (playerActor == OverlappingPlayer)
				{
					OverlappingPlayer->CanInteract = false;
					IsMovingUp = false;
					OverlappingPlayer = nullptr;
				}
			}
		}

	}
}

void ACrankliftTrigger::ServerPlaySound_Implementation(bool _IsPaused)
{
	ClientPlaySound(_IsPaused);
}

void ACrankliftTrigger::ClientPlaySound_Implementation(bool _IsPaused)
{
	if (AudioComponent)
	{
		AudioComponent->SetPaused(_IsPaused);
	}
}