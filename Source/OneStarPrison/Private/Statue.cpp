// Fill out your copyright notice in the Description page of Project Settings.


#include "Statue.h"
#include "StatueManager.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AStatue::AStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));

	RootComponent = Mesh;
	LightMesh->SetupAttachment(RootComponent);

	AnimalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnimalMesh"));
	AnimalMesh->SetupAttachment(RootComponent);

	Mesh0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh0"));
	Mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
	Mesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	Mesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh3"));
	Mesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh4"));
	Mesh5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh5"));
	Mesh6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh6"));
	Mesh7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh7"));
	Mesh8 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh8"));

	Mesh0->SetupAttachment(RootComponent);
	Mesh1->SetupAttachment(RootComponent);
	Mesh2->SetupAttachment(RootComponent);
	Mesh3->SetupAttachment(RootComponent);
	Mesh4->SetupAttachment(RootComponent);
	Mesh5->SetupAttachment(RootComponent);
	Mesh6->SetupAttachment(RootComponent);
	Mesh7->SetupAttachment(RootComponent);
	Mesh8->SetupAttachment(RootComponent);

	Meshes.Add(Mesh0);
	Meshes.Add(Mesh1);
	Meshes.Add(Mesh2);
	Meshes.Add(Mesh3);
	Meshes.Add(Mesh4);
	Meshes.Add(Mesh5);
	Meshes.Add(Mesh6);
	Meshes.Add(Mesh7);
	Meshes.Add(Mesh8);
}

// Called when the game starts or when spawned
void AStatue::BeginPlay()
{
	Super::BeginPlay();

	if (IsRandom)
	{
		if (HasAuthority())
		{
			//Randomly choose one barrel to place the key in
			TopRow = FMath::RandRange(0, 3 - 1);
			//Randomly choose one barrel to place the key in
			MidRow = FMath::RandRange(3, 6 - 1);
			//Randomly choose one barrel to place the key in
			BotRow = FMath::RandRange(6, 9 - 1);
		}

	}

	if (IsClone)
	{
		UpdateVisibility();
		return;
	}

	if (!LightMesh->GetStaticMesh())
	{
		//Show steps only if it doenst light up
		UpdateVisibility();
	}
	else
	{
		for (int i = 0; i < Meshes.Num(); i++)
		{
			if (Meshes[i])
			{
				Meshes[i]->SetVisibility(false);
			}
		}
	}
}

void AStatue::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStatue, TopRow);
	DOREPLIFETIME(AStatue, MidRow);
	DOREPLIFETIME(AStatue, BotRow);

	DOREPLIFETIME(AStatue, IsRandom);
	DOREPLIFETIME(AStatue, Manager);

	DOREPLIFETIME(AStatue, IsClone);
	DOREPLIFETIME(AStatue, IsChosen);
}

// Called every frame
void AStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (IsChosen)
		{
			LightMesh->SetVisibility(true, true);
		}
		else
		{
			LightMesh->SetVisibility(false, true);
		}
	}

}

void AStatue::UpdateVisibility()
{
	for (int i = 0; i < Meshes.Num(); i++)
	{
		if (Meshes[i])
		{
			Meshes[i]->SetVisibility(false);
		}
	}

	Meshes[TopRow]->SetVisibility(true);
	Meshes[MidRow]->SetVisibility(true);
	Meshes[BotRow]->SetVisibility(true);
}