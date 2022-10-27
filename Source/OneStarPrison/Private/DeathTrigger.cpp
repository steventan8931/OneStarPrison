// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Pushable.h"

#include "Pickupable.h"

// Sets default values
ADeathTrigger::ADeathTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADeathTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ADeathTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeathTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeathTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		//If the other actor is a player, kill the player
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
		if (playerActor)
		{
			playerActor->IsDead = true;

			if (DeathSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
			}
		}

		//If the other actor is a pushable, reset its transform
		APushable* pushable = Cast<APushable>(OtherActor);
		if (pushable)
		{
			//Set its simulating physics to false to allow it to be moved
			pushable->Mesh->SetSimulatePhysics(false);
			//Reset variables
			pushable->HasBeenPushed = false;
			//Set transform to starting position
			pushable->SetActorTransform(pushable->StartingTransform);
			//Re-enable physics
			pushable->Mesh->SetSimulatePhysics(true);
		}
		
		//If the other actor is a pushable, reset its transform
		APickupable* pickup = Cast<APickupable>(OtherActor);
		if (pickup)
		{
			if (pickup->OnDisplay)
			{
				pickup->Timer = pickup->TimeBeforeItRespawns;
			}
		}
	}
}
