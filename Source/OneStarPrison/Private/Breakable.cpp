// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

//Reverse Array
#include "Algo/Reverse.h"

#include "Pickupable.h"

#include "Kismet/GameplayStatics.h"

//Particles
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "TrapRoomTrigger.h"

// Sets default values
ABreakable::ABreakable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 100));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	Particles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particles"));
	Particles->SetupAttachment(RootComponent);

	Mesh->OnComponentHit.AddDynamic(this, &ABreakable::OnCompHit);

}

// Called when the game starts or when spawned
void ABreakable::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABreakable::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABreakable::OnOverlapEnd);

	CurrentHealth = MaxHealth;
}

// Called every frame
void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the particles are visible, after a delay disable them
	if (Particles->IsVisible())
	{
		ParticleTimer += DeltaTime;
		
		if (ParticleTimer >= ParticleLifeTime)
		{
			ParticleTimer = 0.0f;
			Particles->SetVisibility(false);
		}
	}


	if (OverlappingPlayer != nullptr)
	{
		//If the player's last punch has been longer than the punch delay
		if (OverlappingPlayer->PunchTimer >= OverlappingPlayer->PunchDelay)
		{
			OverlappingPlayer->CanInteract = true;

			if (OverlappingPlayer->CanInteract)
			{
				//If the player is interacting, update the health and reset the players punch/interact timer
				if (OverlappingPlayer->IsInteracting)
				{
					UpdateHealth();
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->PunchTimer = 0.0f;
				}
			}
		}
		else
		{
			//Don't allow the overlapping player to interact
			OverlappingPlayer->IsInteracting = false;
			OverlappingPlayer->CanInteract = false;
		}



	}

	//If the health is less or equal to zero, destroy the actor after a delay
	if (CurrentHealth <= 0)
	{
		DestroyTimer += DeltaTime;

		if (DestroyTimer >= TimeToDestroy)
		{
			Destroy();
		}

	}

}

void ABreakable::UpdateHealth()
{
	if (CurrentHealth > 0)
	{
		//Player the hit sound, and show the breaking particles
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
			Particles->SetVisibility(true);
			ParticleTimer = 0.0f;
		}

		//Decrement the health of the breakable by the damage per hit
		CurrentHealth -= DamagePerHit;

		//If the breakable it out of health, play the break shown and spawn an actor or debris
		//if it is the server and those objects exists
		if (CurrentHealth <= 0)
		{
			if (BreakSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSound, GetActorLocation());
				if (HasAuthority())
				{
					if (ActorToSpawn)
					{
						SpawnActor(ActorToSpawn);
					}

					if (DebrisToSpawn)
					{
						SpawnActor(DebrisToSpawn);
					}
				}

			}
		}
	}
	else
	{
		return;
	}

	//Deactive the break particles
	Particles->Activate(false);

}

void ABreakable::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			if (!IsBreakableByProjectiles)
			{
				//If it is intended to be broken by the player, allow that player to interact 
				APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

				if (playerActor)
				{
					OverlappingPlayer = playerActor;
					OverlappingPlayer->CanInteract = true;
					//Change the players interact type to punching
					OverlappingPlayer->InteractType = EInteractType::Punch;
				}
			}
			else
			{
				//If it is breakable by projectiles check if it is on pickupable class
				APickupable* pickupable = Cast<APickupable>(OtherActor);

				//If it is of pickupable class, update the breakable
				if (pickupable)
				{
					UpdateHealth();
				}
			}
		}
	}
}

void ABreakable::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		if (playerActor)
		{
			//If there is an overlapping player and if they player is leaving actor, make them unable to interact and set overlapping to nullptr
			if (OverlappingPlayer != nullptr)
			{
				if (playerActor == OverlappingPlayer)
				{
					OverlappingPlayer->CanInteract = false;

					OverlappingPlayer = nullptr;
				}

			}
		}

	}
}

AActor* ABreakable::SpawnActor(TSubclassOf<class AActor> _Actor)
{
	return(GetWorld()->SpawnActor<AActor>(_Actor, GetActorTransform()));
}

void ABreakable::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//If the player hits it, allow them to interact with it
	if ((OtherActor) && (OtherActor != this))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			//If the player's last punch has been longer than the punch delay
			if (player->PunchTimer >= player->PunchDelay)
			{
				player->CanInteract = true;
				if (OverlappingPlayer)
				{
					OverlappingPlayer->CanInteract = false;
				}
				OverlappingPlayer = player;
			}
			//Change the players interact type to punching
			player->InteractType = EInteractType::Punch;
		}
	}
}