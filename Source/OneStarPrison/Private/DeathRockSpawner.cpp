// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathRockSpawner.h"
#include "DeathRock.h"

// Sets default values
ADeathRockSpawner::ADeathRockSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADeathRockSpawner::BeginPlay()
{
	Super::BeginPlay();
	ADeathRock* rock = Cast<ADeathRock>(SpawnActor());
	if (rock)
	{
		rock->EndPosition = EndPosition;
		rock->DeathTimer = SpawnRate;
	}
}

void ADeathRockSpawner::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called every frame
void ADeathRockSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimer += DeltaTime;

	if (SpawnTimer >= SpawnRate)
	{
		ADeathRock* rock = Cast<ADeathRock>(SpawnActor());
		if (rock)
		{
			rock->EndPosition = EndPosition;
			rock->DeathTimer = SpawnRate;
		}
		SpawnTimer = 0.0f;
	}
}

AActor* ADeathRockSpawner::SpawnActor()
{
	return(GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorTransform()));
}

