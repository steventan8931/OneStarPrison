// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, OpenPosition);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		OpenDoor(DeltaTime);
		return;
	}
}

void ADoor::OpenDoor(float _DeltaTime)
{
	if (Mesh->GetComponentLocation() != OpenPosition)
	{
		FVector newPos = FMath::Lerp(Mesh->GetComponentLocation(), OpenPosition, _DeltaTime);
		Mesh->SetWorldLocation(newPos);
	}

}

void ADoor::PlaySound()
{
	if (OpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
	}
}