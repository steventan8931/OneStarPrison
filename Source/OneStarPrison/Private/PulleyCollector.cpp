// Fill out your copyright notice in the Description page of Project Settings.


#include "PulleyCollector.h"
#include "PulleyPlatform.h"
#include "Components/BoxComponent.h"
#include "Pickupable.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
APulleyCollector::APulleyCollector()
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

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APulleyCollector::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APulleyCollector::OnOverlapEnd);
}

void APulleyCollector::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APulleyCollector, Platform);
	DOREPLIFETIME(APulleyCollector, RockCount);
	DOREPLIFETIME(APulleyCollector, HeightPerRock);

	DOREPLIFETIME(APulleyCollector, StartingHeight);
	DOREPLIFETIME(APulleyCollector, TargetHeight);
}

// Called when the game starts or when spawned
void APulleyCollector::BeginPlay()
{
	Super::BeginPlay();
	
	StartingHeight = GetActorLocation().Z;
	UpdateTargetPos();
}

// Called every frame
void APulleyCollector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Move the platform based on its target height
	MovePlatform(DeltaTime);
}

void APulleyCollector::MovePlatform(float _DeltaTime)
{
	//If the platform exists
	if (Platform)
	{
		//Move the platform to its target height
		//If the target height of the platform is lower than the current height, move the platform down
		if (Platform->GetActorLocation().Z > Platform->TargetHeight)
		{
			Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z - _DeltaTime * MoveSpeed));
		}	
		//If the target height of the platform is higher than the current height, move the platform up
		if (Platform->GetActorLocation().Z < Platform->TargetHeight)
		{
			Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z + _DeltaTime * MoveSpeed));
		}

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

void APulleyCollector::UpdateTargetPos()
{
	//If the platform exists
	if (Platform)
	{
		//If the platforms estimated target height is less than its max height
		if ((Platform->StartingHeight + (RockCount * HeightPerRock)) <= Platform->MaxHeight)
		{
			//Set the estimated height as the platform's target hegith
			Platform->TargetHeight = Platform->StartingHeight + (RockCount * HeightPerRock);

			//Set the target height of the pulley as the inverse 
			TargetHeight = StartingHeight - (RockCount * HeightPerRock);
		}
	}
}

void APulleyCollector::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APickupable* pickupable = Cast<APickupable>(OtherActor);

		//If the overlapping actor is a pickupable
		if (pickupable)
		{
			//If the pickupable is not simulating physics (while its in the air)
			if (!pickupable->Mesh->IsSimulatingPhysics())
			{
				//Have the sever play the sound
				ServerPlaySound();
			}

			//Make the pickupable simulate physics
			pickupable->Mesh->SetSimulatePhysics(true);
			//Disables it projectile movement
			pickupable->ProjectileMovement->Deactivate();

			//If the platform is valid
			if (Platform)
			{
				//Increment the rock count
				RockCount++;
				//Update the target position of the platform and the pulley
				UpdateTargetPos();
			}

		}
	}
}

void APulleyCollector::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APickupable* pickupable = Cast<APickupable>(OtherActor);

		//If the overlapping actor is a pickupable
		if (pickupable)
		{
			//Set its velocity to zero
			pickupable->ProjectileMovement->Velocity = FVector(0, 0, 0);

			//If the platform is valid
			if (Platform)
			{
				//Decrement the rock count
				RockCount--;
				//Update the target position of the platform and pulley
				UpdateTargetPos();
			}

		}
	}
}

void APulleyCollector::ServerPlaySound_Implementation()
{
	//Have the server play the sound on all client
	ClientPlaySound();
}

void APulleyCollector::ClientPlaySound_Implementation()
{
	if (MovingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MovingSound, GetActorLocation());
	}
}