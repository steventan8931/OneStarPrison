// Fill out your copyright notice in the Description page of Project Settings.


#include "Mannequin.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	HelmetPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HelmetPosition"));
	HelmetPosition->SetupAttachment(Mesh);

	ArmorPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ArmorPosition"));
	ArmorPosition->SetupAttachment(Mesh);

	FootwearPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FootwearPosition"));
	FootwearPosition->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

