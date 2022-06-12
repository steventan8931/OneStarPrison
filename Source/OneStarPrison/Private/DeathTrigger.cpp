// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

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

			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
			if (playerActor)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Dead"));
				playerActor->IsDead = true;
			}
		
	}
}
