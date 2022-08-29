// Fill out your copyright notice in the Description page of Project Settings.


#include "MannequinArmor.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Called when the game starts or when spawned
void AMannequinArmor::BeginPlay()
{
	Super::BeginPlay();
}

void AMannequinArmor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMannequinArmor, AttachedToMannequin);
}

// Called every frame
void AMannequinArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}