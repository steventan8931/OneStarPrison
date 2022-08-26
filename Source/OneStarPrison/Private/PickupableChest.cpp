// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableChest.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Called when the game starts or when spawned
void APickupableChest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickupableChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the chest is being held by a player, make the player's holding heavy item to true
	if (Player)
	{
		Player->IsHoldingHeavyItem = true;
	}
}