// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleDoorCastle.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "PickupableKey.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoubleDoorCastle::ADoubleDoorCastle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	RootComponent = BoxCollision;

	LMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LMesh"));
	LMesh->SetupAttachment(RootComponent);

	RMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RMesh"));
	RMesh->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoubleDoorCastle::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ADoubleDoorCastle::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ADoubleDoorCastle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoubleDoorCastle::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoubleDoorCastle, LOpenRotation);
	DOREPLIFETIME(ADoubleDoorCastle, ROpenRotation);
	DOREPLIFETIME(ADoubleDoorCastle, NumOfOverlappingPlayers);
}


// Called every frame
void ADoubleDoorCastle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		OpenDoor(DeltaTime);
		return;
	}

	if (NumOfOverlappingPlayers == 2)
	{
		IsOpen = true;
		if (OpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
		}
	}

}

void ADoubleDoorCastle::OpenDoor(float _DeltaTime)
{
	if (LMesh->GetComponentRotation() != LOpenRotation)
	{
		FRotator newRotL = FMath::Lerp(LMesh->GetRelativeRotation(), LOpenRotation, _DeltaTime);
		LMesh->SetRelativeRotation(newRotL);
	}

	if (RMesh->GetComponentRotation() != ROpenRotation)
	{
		FRotator newRotR = FMath::Lerp(RMesh->GetRelativeRotation(), ROpenRotation, _DeltaTime);
		RMesh->SetRelativeRotation(newRotR);
	}
}

void ADoubleDoorCastle::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				NumOfOverlappingPlayers++;
				OverlappingPlayer = playerActor;

			}
		}
		else
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				NumOfOverlappingPlayers++;
				OverlappingPlayer2 = playerActor;
			}
		}
	}
}

void ADoubleDoorCastle::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		//if (OverlappingPlayer != nullptr)
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		if (playerActor)
		{
			if (OverlappingPlayer == playerActor)
			{
				OverlappingPlayer = nullptr;
			}
			if (OverlappingPlayer2 == playerActor)
			{
				OverlappingPlayer2 = nullptr;
			}

			NumOfOverlappingPlayers--;
		}

	}
}