// Fill out your copyright notice in the Description page of Project Settings.


#include "PulleyDouble.h"
#include "Components/BoxComponent.h"
#include "Pickupable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Pushable.h"
#include "Breakable.h"

#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
APulleyDouble::APulleyDouble()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APulleyDouble::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APulleyDouble::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APulleyDouble::BeginPlay()
{
	Super::BeginPlay();
	StartingHeight = GetActorLocation().Z;
	UpdateTargetPos();
}

void APulleyDouble::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APulleyDouble, IsSoundPlaying);
	DOREPLIFETIME(APulleyDouble, SoundTimer);
	DOREPLIFETIME(APulleyDouble, SoundReplayTimer);
}



// Called every frame
void APulleyDouble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTargetPos();
	MovePlatform(DeltaTime);

	if (BreakablePlatform)
	{
		//if (BreakablePlatform->CurrentHealth == 0)
		{
			OtherPlatform->RockCount += 10;
			MaxHeight = MaxHeightAfterBreaking;
			BreakablePlatform = nullptr;
		}
	}

	if (IsSoundPlaying)
	{
		SoundTimer += DeltaTime;

		if (SoundTimer >= SoundReplayTimer)
		{
			IsSoundPlaying = false;
		}
	}
	else
	{
		SoundTimer = 0.0f;
	}
}

void APulleyDouble::MovePlatform(float _DeltaTime)
{
	if (OtherPlatform)
	{
		if (GetActorLocation().Z > TargetHeight)
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - _DeltaTime * MoveSpeed));
		}
		if (GetActorLocation().Z < TargetHeight)
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + _DeltaTime * MoveSpeed));
		}
	}
}

void APulleyDouble::UpdateTargetPos()
{
	if (OtherPlatform)
	{
		if ((OtherPlatform->StartingHeight + (RockCount * HeightPerRock) - (OtherPlatform->RockCount * HeightPerRock)) <= OtherPlatform->MaxHeight)
		{
			OtherPlatform->TargetHeight = OtherPlatform->StartingHeight + (RockCount * HeightPerRock) - (OtherPlatform->RockCount * HeightPerRock);
			TargetHeight = StartingHeight - (RockCount * HeightPerRock) + (OtherPlatform->RockCount * HeightPerRock);
		}
	}
}

void APulleyDouble::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APushable* pushable = Cast<APushable>(OtherActor);

		if (pushable)
		{
			if (OtherPlatform)
			{
				if (!pushable->HasBeenPushed)
				{
					if (!IsSoundPlaying)
					{
						ServerPlaySound();
						IsSoundPlaying = true;
					}

					pushable->HasBeenPushed = true;
				}


				RockCount += pushable->PulleyWeightage;
			}

		}

	}
}

void APulleyDouble::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{

		APushable* pushable = Cast<APushable>(OtherActor);

		if (pushable)
		{
			if (OtherPlatform)
			{
				if (MovingSound)
				{
					//if (!pushable->HasBeenPushed)
					{
						if (!IsSoundPlaying)
						{
							ServerPlaySound();
							IsSoundPlaying = true;
						}
						pushable->HasBeenPushed = true;
					}
				}
				RockCount -= pushable->PulleyWeightage;
			}

		}
	}
}

void APulleyDouble::ServerPlaySound_Implementation()
{
	ClientPlaySound();
}

void APulleyDouble::ClientPlaySound_Implementation()
{
	if (MovingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MovingSound, GetActorLocation());
	}
}