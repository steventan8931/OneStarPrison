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

	for (int Index = 0; Index != ListOfMaterialsAtHealth.Num(); ++Index)
	{
		if (CurrentHealth == ListOfMaterialsAtHealth[Index].AtHP)
		{
			if (ListOfMaterialsAtHealth[Index].Material != nullptr)
			{
				//Mesh->SetMaterial(0, ListOfMaterialsAtHealth[Index].Material);
			}
		}
	}
}

// Called every frame
void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		if (OverlappingPlayer->PunchTimer >= OverlappingPlayer->PunchDelay)
		{
			OverlappingPlayer->CanInteract = true;
			if (OverlappingPlayer->CanInteract)
			{
				if (OverlappingPlayer->IsInteracting)
				{
					HitOnce = true;
					UpdateMaterial();
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->PunchTimer = 0.0f;
				}
				else
				{
					HitOnce = false;
				}
			}
			//else
			//{
			//	OverlappingPlayer->IsInteracting = false;
			//	OverlappingPlayer->CanInteract = true;
			//}
		}
		else
		{
			OverlappingPlayer->IsInteracting = false;
			OverlappingPlayer->CanInteract = false;
		}



	}

	if (CurrentHealth <= 0)
	{
		DestroyTimer += DeltaTime;

		if (DestroyTimer >= TimeToDestroy)
		{
			Destroy();
		}

	}

}

void ABreakable::UpdateMaterial()
{
	if (CurrentHealth > 0)
	{
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
			Particles->SetVisibility(true);
			ParticleTimer = 0.0f;
		}
		CurrentHealth -= DamagePerHit;
		if (CurrentHealth <= 0)
		{
			if (BreakSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSound, GetActorLocation());
				//GEngine->AddOnScreenDebugMessage(-1, 11.0f, FColor::Yellow, FString::Printf(TEXT("BREAK SOUND PLAY = %i"), CurrentHealth));
				if (HasAuthority())
				{
					if (ActorToSpawn)
					{
						SpawnActor(ActorToSpawn);
					}

					if (DeprisToSpawn)
					{
						SpawnActor(DeprisToSpawn);
					}
				}

			}
		}
	}
	else
	{
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Current Health = %i"), CurrentHealth));

	for (int Index = 0; Index != ListOfMaterialsAtHealth.Num(); ++Index)
	{
		if(CurrentHealth == ListOfMaterialsAtHealth[Index].AtHP)
		{
			if (ListOfMaterialsAtHealth[Index].Material != nullptr)
			{
				//Mesh->SetMaterial(0, ListOfMaterialsAtHealth[Index].Material);
			}
		}
	}

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
				APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

				if (playerActor)
				{
					OverlappingPlayer = playerActor;
					//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, playerActor->GetName());
					OverlappingPlayer->CanInteract = true;
					OverlappingPlayer->InteractType = EInteractType::Punch;
				}
			}
			else
			{
				APickupable* pickupable = Cast<APickupable>(OtherActor);

				if (pickupable)
				{
					UpdateMaterial();
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	if ((OtherActor) && (OtherActor != this))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			player->CanInteract = true;
			player->InteractType = EInteractType::Punch;
		}
	}
}