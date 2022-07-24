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
				//AudioComponent->SetIsReplicated(true);
				//if (!HasAuthority())
				//{
				//	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("moving"));
				//};
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
}

// Called every frame
void ACrankliftTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	cacheDeltaTime = DeltaTime;

	if (Platform)
	{
		if (OverlappingPlayer != nullptr)
		{
			if (OverlappingPlayer->IsInteracting)
			{
				if (AudioComponent)
				{
					if (!AudioComponent->IsPlaying())
					{
						//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("not playing"));

						ServerPlaySound(false);
					}
					else
					{
						//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("playing"));
					}
				}

				IsMovingUp = true;
			}
			else
			{
				IsMovingUp = false;
			}
		}
		else
		{
			IsMovingUp = false;
		}

		if (IsMovingUp)
		{
			if (Platform->GetActorLocation().Z <= MaxHeight)
			{
				MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime));
				Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z + cacheDeltaTime * MoveSpeed));

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
			if (Platform->GetActorLocation().Z >= MinHeight)
			{
				MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
				Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z - cacheDeltaTime * MoveSpeed));

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

			if (playerActor)
			{
				OverlappingPlayer = playerActor;
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, playerActor->GetName());
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
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
			if (playerActor)
			{
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