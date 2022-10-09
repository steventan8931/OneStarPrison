// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableKey.h"
#include "AI/NPC.h"

// Called when the game starts or when spawned
void APickupableKey::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickupableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupableKey::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor) && (OtherActor != this))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

		ANPC* enemy = Cast<ANPC>(OtherActor);

		//If the mesh hits a player that isn't the player and isnt
		if (!player )
		{
			//If the key is owned by the guard and hit the key
			if (OwnedByGuard && !enemy)
			{
				//Dont make it simulate physics
				return;
			}

			//Otherwise Set its physics to true and disables its projectile movement
			IsInAir = false;
			Mesh->SetSimulatePhysics(true);
			ProjectileMovement->Deactivate();
			Mesh->SetCollisionProfileName("BlockAll");
		}

	}
}