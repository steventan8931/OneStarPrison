// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawbridgeTrigger.h"
#include "DrawbridgePlatform.h"
#include "Components/BoxComponent.h"
#include "Pickupable.h"

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
}

// Called when the game starts or when spawned
void ADrawbridgeTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrawbridgeTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrawbridgeTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APickupable* pickupable = Cast<APickupable>(OtherActor);

		if (pickupable)
		{
			if (Platform)
			{
				Platform->IsOpen = true;
			}

		}
	}
}

