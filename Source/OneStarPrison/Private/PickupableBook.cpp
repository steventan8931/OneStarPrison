// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableBook.h"

// Called when the game starts or when spawned
void APickupableBook::BeginPlay()
{
	Super::BeginPlay();

	cacheTransform = GetActorTransform();

	ParentActor = GetParentActor();
}

// Called every frame
void APickupableBook::Tick(float DeltaTime)
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


		if (Timer > 1.0f)
		{

			TakenFromDisplay = false;
			if (ParentActor)
			{
				AttachToActor(ParentActor, FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
			SetActorTransform(cacheTransform);
			Mesh->SetSimulatePhysics(false);
			Timer = 0.0f;
		}

	}
}