// Fill out your copyright notice in the Description page of Project Settings.


#include "PulleyPlatform.h"

// Sets default values
APulleyPlatform::APulleyPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void APulleyPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartingHeight = GetActorLocation().Z;
}

// Called every frame
void APulleyPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Tick is turned off due to it not needing to call the update function
}

