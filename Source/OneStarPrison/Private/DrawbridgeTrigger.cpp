// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawbridgeTrigger.h"
#include "DrawbridgePlatform.h"
#include "Components/BoxComponent.h"
#include "Pickupable.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
ADrawbridgeTrigger::ADrawbridgeTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("Trigger"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADrawbridgeTrigger::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ADrawbridgeTrigger::OnOverlapEnd);

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	MovableMesh->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ADrawbridgeTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADrawbridgeTrigger::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADrawbridgeTrigger, HandleOpenRotation);
	DOREPLIFETIME(ADrawbridgeTrigger, HandleClosedRotation);
}


// Called every frame
void ADrawbridgeTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the drawbridge has been linked
	if (Platform)
	{
		//If the platform isn't open yet
		if (!Platform->IsOpen)
		{
			if (OverlappingPlayer)
			{
				//If the overlapping player is interacting
				if (OverlappingPlayer->IsInteracting)
				{
					//Open the platform and play the open sound
					Platform->IsOpen = true;
					if (OpenSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
					}

					//Update the position of the handle 
					MovableMesh->SetRelativeRotation(HandleOpenRotation);
					//Dont let the player interact again
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->IsInteracting = false;
				}
			}
		}
		else
		{
			//Update the position of the handle
			MovableMesh->SetRelativeRotation(HandleClosedRotation);
		}

	}
}

void ADrawbridgeTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		//If it is to interacted by throwing projectiles
		if (!IsPlayerInteractable)
		{

			APickupable* pickupable = Cast<APickupable>(OtherActor);
			//If the overlapping player is a pickupable
			if (pickupable)
			{
				if (Platform)
				{
					//Open the platform and play the open sound
					Platform->IsOpen = true;
					if (OpenSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
					}
				}

			}
		}
		else
		{
			//If it is meant to be interacted by players
			if (OverlappingPlayer == nullptr)
			{
				if (Platform)
				{
					if (!Platform->IsOpen)
					{
						APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

						//If the overlapping player is a player
						if (playerActor)
						{
							//Set the overlapping player to this player and allow them to interact
							OverlappingPlayer = playerActor;
							OverlappingPlayer->CanInteract = true;
							//Change the players interact type to lever pulling
							OverlappingPlayer->InteractType = EInteractType::LeverPull;

						}
					}
				}
			}
		}

	}
}

void ADrawbridgeTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If the overlapping player exists
			if (OverlappingPlayer)
			{
				//If this overlapping player is the overlapping player
				if (playerActor == OverlappingPlayer)
				{
					//Remove the players ability to interact with the lever
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer = nullptr;
				}
			}
		}

	}
}
