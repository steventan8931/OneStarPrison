// Fill out your copyright notice in the Description page of Project Settings.


#include "PulleyDouble.h"
#include "Components/BoxComponent.h"
#include "Pickupable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Pushable.h"
#include "Breakable.h"

#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

#include "PickupableChest.h"

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
	//Set the starting height of the pulleys to their current z positions
	StartingHeight = GetActorLocation().Z;
	//Update their target positions based on their starting heights
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

	//Update the target position of the pulleys
	UpdateTargetPos();

	//Move the pulleys to their new target heights
	MovePlatform(DeltaTime);

	//If the sound has been played
	if (IsSoundPlaying)
	{
		//Increment the sound timer
		SoundTimer += DeltaTime;

		//If the sound timer has pasted the replay timer
		if (SoundTimer >= SoundReplayTimer)
		{
			//Reset the is sound playing bool
			IsSoundPlaying = false;
		}
	}
	else
	{
		//If the sound hasn't been played set sound timer to 0
		SoundTimer = 0.0f;
	}
}

void APulleyDouble::MovePlatform(float _DeltaTime)
{
	//If the other platform is valid
	if (OtherPlatform)
	{
		//If the target height of the pulley is less than than the current height, move the pulley down
		if (GetActorLocation().Z > TargetHeight)
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - _DeltaTime * MoveSpeed));
		}
		//If the target height of the pulley is higher than than the current height, move the pulley up
		if (GetActorLocation().Z < TargetHeight)
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + _DeltaTime * MoveSpeed));
		}
	}
}

void APulleyDouble::UpdateTargetPos()
{
	//If the other platform is valid
	if (OtherPlatform)
	{
		//If the pulley estimated target height is less than its max height
		if ((OtherPlatform->StartingHeight + (RockCount * HeightPerRock) - (OtherPlatform->RockCount * HeightPerRock)) <= OtherPlatform->MaxHeight)
		{
			//Set the estimated height as the other pulley's target height
			OtherPlatform->TargetHeight = OtherPlatform->StartingHeight + (RockCount * HeightPerRock) - (OtherPlatform->RockCount * HeightPerRock);

			//Set the target height of the pulley as the inverse 
			TargetHeight = StartingHeight - (RockCount * HeightPerRock) + (OtherPlatform->RockCount * HeightPerRock);
		}
	}
}

void APulleyDouble::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APickupableChest* barrel = Cast<APickupableChest>(OtherActor);

		//If the overlappign actor is a pushable
		if (barrel)
		{
			if (OtherPlatform)
			{
				//If the sound isnt playing
				if (!IsSoundPlaying)
				{
					ServerPlaySound();
					IsSoundPlaying = true;
				}

				//Increment the rock count by the weightage of the pushable
				RockCount += 7;
			}
		}

	}
}

void APulleyDouble::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APickupableChest* barrel = Cast<APickupableChest>(OtherActor);

		//If the overlappign actor is a pushable
		if (barrel)
		{
			if (OtherPlatform)
			{
				//If the sound isnt playing
				if (!IsSoundPlaying)
				{
					ServerPlaySound();
					IsSoundPlaying = true;
				}
				
				//Decrement the rock count by the weightage of the pushable
				RockCount -= 7;
			}

		}
	}
}

void APulleyDouble::ServerPlaySound_Implementation()
{
	//Have the server play the sound on all clients
	ClientPlaySound();
}

void APulleyDouble::ClientPlaySound_Implementation()
{
	if (MovingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MovingSound, GetActorLocation());
	}
}