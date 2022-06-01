// Fill out your copyright notice in the Description page of Project Settings.


#include "PulleyCollector.h"
#include "PulleyPlatform.h"
#include "Components/BoxComponent.h"
#include "Pickupable.h"

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

// Called when the game starts or when spawned
void APulleyCollector::BeginPlay()
{
	Super::BeginPlay();
	
	StartingHeight = GetActorLocation().Z;
}

// Called every frame
void APulleyCollector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovePlatform(DeltaTime);
}

void APulleyCollector::MovePlatform(float _DeltaTime)
{
	if (Platform)
	{
		if (Platform->GetActorLocation().Z > Platform->TargetHeight)
		{
			Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z - _DeltaTime * MoveSpeed));
		}	
		if (Platform->GetActorLocation().Z < Platform->TargetHeight)
		{
			Platform->SetActorLocation(FVector(Platform->GetActorLocation().X, Platform->GetActorLocation().Y, Platform->GetActorLocation().Z + _DeltaTime * MoveSpeed));
		}

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

void APulleyCollector::UpdateTargetPos()
{
	Platform->TargetHeight = Platform->StartingHeight + (RockCount * HeightPerRock);

	TargetHeight = StartingHeight - (RockCount * HeightPerRock);
}

void APulleyCollector::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APickupable* pickupable = Cast<APickupable>(OtherActor);

		if (pickupable)
		{
			if (Platform)
			{
				RockCount++;
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

		if (pickupable)
		{
			if (Platform)
			{
				RockCount--;
				UpdateTargetPos();
			}

		}
	}
}