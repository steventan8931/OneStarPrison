// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathRockSpawner.h"
#include "DeathRock.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

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

	DOREPLIFETIME(ADeathRockSpawner, EndPosition);
	DOREPLIFETIME(ADeathRockSpawner, SpawnRate);
	DOREPLIFETIME(ADeathRockSpawner, SpawnTimer);
}

// Called every frame
void ADeathRockSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsActive)
	{
		return;
	}

	SpawnTimer += DeltaTime;

	if (SpawnTimer >= SoundPlayTimer)
	{
		if (!SoundPlayed)
		{
			if (PreSpawnSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), PreSpawnSound, GetActorLocation());
			}
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("playg"));
		}

		SoundPlayed = true;
	}

	//If the timer has been longer than the spawn delay
	if (SpawnTimer >= SpawnRate)
	{
		if (ActorToSpawn)
		{
			ADeathRock* rock = Cast<ADeathRock>(SpawnActor());

			//Initialise the spawn rock variables
			if (rock)
			{
				rock->EndPosition = EndPosition;
				rock->DeathTimer = SpawnRate;
			}

			SoundPlayed = false;

			//Reset the spawn timer
			SpawnTimer = 0.0f;
		}
	}
}

AActor* ADeathRockSpawner::SpawnActor()
{
	return(GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorTransform()));
}

