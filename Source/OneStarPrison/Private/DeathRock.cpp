// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathRock.h"
#include "PlayerCharacter.h"

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

// Called every frame
void ADeathRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DeathTime += DeltaTime;

	if (GetActorLocation() != EndPosition)
	{
		FVector newPos = FMath::Lerp(GetActorLocation(), EndPosition, DeltaTime * MoveSpeed);
		SetActorLocation(newPos);
	}
	else
	{
		Destroy();
	}

	if (DeathTime > DeathTimer)
	{
		Destroy();
	}
	
}

void ADeathRock::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	if ((OtherActor) && (OtherActor != this))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			player->IsDead = true;
		}
	}
}