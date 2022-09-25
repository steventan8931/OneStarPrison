// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathRock.h"
#include "PlayerCharacter.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>


// Sets default values
ADeathRock::ADeathRock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->OnComponentHit.AddDynamic(this, &ADeathRock::OnCompHit);
}

// Called when the game starts or when spawned
void ADeathRock::BeginPlay()
{
	Super::BeginPlay();

}

void ADeathRock::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeathRock, EndPosition);

	DOREPLIFETIME(ADeathRock, IsStatic);
}
// Called every frame
void ADeathRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsStatic)
	{
		return;
	}

	DeathTime += DeltaTime;

	//If the rock isn't at its end position, make the rock move towards the end position
	if (GetActorLocation() != EndPosition)
	{
		FVector newPos = FMath::Lerp(GetActorLocation(), EndPosition, DeltaTime * MoveSpeed);
		SetActorLocation(newPos);
	}
	else
	{
		//Otherwise destroy the rock
		Destroy();
	}

	//If the rock has been spawned for longer than the death timer, destroy the rock
	if (DeathTime > DeathTimer)
	{
		Destroy();
	}
	
}

void ADeathRock::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor) && (OtherActor != this))
	{
		//If the actor hit is a player, kill the player
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			player->IsDead = true;
		}
	}
}