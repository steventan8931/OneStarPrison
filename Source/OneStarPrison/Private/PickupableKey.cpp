// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableKey.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

void APickupableKey::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
