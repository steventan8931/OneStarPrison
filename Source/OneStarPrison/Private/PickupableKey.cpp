// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableKey.h"

// Called when the game starts or when spawned
void APickupableKey::BeginPlay()
{
	Super::BeginPlay();

	cacheTransform = GetActorTransform();

	ParentActor = GetParentActor();
}

// Called every frame
void APickupableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OnDisplay)
	{
		if (Player)
		{
			TakenFromDisplay = true;
		}
		else
		{
			if (TakenFromDisplay)
			{
				if (!IsInAir)
				{
					Timer += DeltaTime;
				}
			}
		}


		if (Timer > TimeBeforeItRespawns)
		{
			TakenFromDisplay = false;
			if (ParentActor)
			{
				AttachToActor(ParentActor, FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
			SetActorTransform(cacheTransform);
			Mesh->SetSimulatePhysics(false);

			if (ReturnToDisplaySound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReturnToDisplaySound, GetActorLocation());
			}

			Timer = 0.0f;
		}

	}
}