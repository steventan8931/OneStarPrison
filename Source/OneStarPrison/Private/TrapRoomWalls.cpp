// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapRoomWalls.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrapRoomWalls::ATrapRoomWalls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 100));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapRoomWalls::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ATrapRoomWalls::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrapRoomWalls::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATrapRoomWalls, OpenPosition);
	DOREPLIFETIME(ATrapRoomWalls, ClosedPosition);
	DOREPLIFETIME(ATrapRoomWalls, IsOpen);
}

// Called every frame
void ATrapRoomWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
}

void ATrapRoomWalls::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
		if (playerActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("hit by wall"));
			playerActor->HitByWallCount += 1;
		}

	}
}

void ATrapRoomWalls::OpenDoor(float _DeltaTime)
{
	if (Mesh->GetComponentLocation() != OpenPosition)
	{
		FVector newPos = FMath::Lerp(Mesh->GetComponentLocation(), OpenPosition, _DeltaTime * MoveSpeed);
		Mesh->SetWorldLocation(newPos);
	}
}

void ATrapRoomWalls::CloseDoor(float _DeltaTime)
{
	Mesh->SetWorldLocation(ClosedPosition);
}

void ATrapRoomWalls::PlaySound()
{
	if (OpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
	}
}