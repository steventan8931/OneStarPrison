// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapRoomWalls.h"

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
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ATrapRoomWalls::OnOverlapEnd);
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

	DOREPLIFETIME(ATrapRoomWalls, MoveSpeed);
}

// Called every frame
void ATrapRoomWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Updates the position of the walls based on if they are open or closed
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
		//If the overlapping actor is a player increment their hit by wall count by 1
		if (playerActor)
		{
			playerActor->HitByWallCount += 1;
		}
	}
}

void ATrapRoomWalls::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
		//If the overlapping actor is a player decrement their hit by wall count by 1
		if (playerActor)
		{
			playerActor->HitByWallCount -= 1;
		}

	}
}

void ATrapRoomWalls::OpenDoor(float _DeltaTime)
{
	if (Mesh->GetComponentLocation() != OpenPosition)
	{
		//Move the doors to its open position over time
		FVector newPos = FMath::Lerp(Mesh->GetComponentLocation(), OpenPosition, _DeltaTime * MoveSpeed);
		Mesh->SetWorldLocation(newPos);
	}
}

void ATrapRoomWalls::CloseDoor(float _DeltaTime)
{
	//Reset the position of the trap wall to its starting position
	Mesh->SetWorldLocation(ClosedPosition);
}

void ATrapRoomWalls::PlaySound()
{
	//Plays the sound
	if (OpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
	}
}