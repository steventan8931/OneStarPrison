// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

//Reverse Array
#include "Algo/Reverse.h"

#include "Pickupable.h"

// Sets default values
ABreakable::ABreakable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

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
				Mesh->SetMaterial(0, ListOfMaterialsAtHealth[Index].Material);
			}
		}
	}
}

// Called every frame
void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingPlayer != nullptr)
	{
		if (OverlappingPlayer->IsInteracting)
		{
			HitOnce = true;
			UpdateMaterial();
			OverlappingPlayer->IsInteracting = false;
		}
		else
		{
			HitOnce = false;
		}
	}

	if (CurrentHealth <= 0)
	{
		Destroy();
	}

}

void ABreakable::UpdateMaterial()
{
	if (CurrentHealth > 0)
	{
		CurrentHealth -= DamagePerHit;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Current Health = %i"), CurrentHealth));

	for (int Index = 0; Index != ListOfMaterialsAtHealth.Num(); ++Index)
	{
		if(CurrentHealth == ListOfMaterialsAtHealth[Index].AtHP)
		{
			if (ListOfMaterialsAtHealth[Index].Material != nullptr)
			{
				Mesh->SetMaterial(0, ListOfMaterialsAtHealth[Index].Material);
			}
		}
	}

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
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, playerActor->GetName());
					OverlappingPlayer->CanInteract = true;
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
				OverlappingPlayer->CanInteract = false;

				OverlappingPlayer = nullptr;

			}
		}

	}
}

