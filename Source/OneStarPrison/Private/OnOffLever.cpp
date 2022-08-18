// Fill out your copyright notice in the Description page of Project Settings.


#include "OnOffLever.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

#include "OnOffDoorLeverManager.h"

// Sets default values
AOnOffLever::AOnOffLever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(150, 150, 150));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AOnOffLever::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AOnOffLever::OnOverlapEnd);

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	MovableMesh->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AOnOffLever::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOnOffLever::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOnOffLever, HandleOpenRotation);
	DOREPLIFETIME(AOnOffLever, HandleClosedRotation);
	DOREPLIFETIME(AOnOffLever, IsOpen);


	DOREPLIFETIME(AOnOffLever, Manager);
	DOREPLIFETIME(AOnOffLever, IndexInManager);
}

// Called every frame
void AOnOffLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingPlayer != nullptr)
	{
		//If the timer has been longer than the delay
		if (OpenTimer >= OpenTime)
		{
			//Allow the player to interact
			OverlappingPlayer->CanInteract = true;
			//Change the players interact type to lever pulling
			OverlappingPlayer->InteractType = EInteractType::LeverPull;
		}

		//If the player is able to interact
		if (OverlappingPlayer->CanInteract)
		{
			//if the player is interacting
			if (OverlappingPlayer->IsInteracting)
			{
				//If the lever is already in its open state
				if (IsOpen)
				{
					//Set its state to closed
					IsOpen = false;

					//Play the lever sound
					if (OpenSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
					}

					
					if (Manager)
					{
						//Iterate through all the doors
						for (int i = 0; i < Manager->OnOffLeverDoors.Num(); i++)
						{
							//Have the doors play their open/close sounds
							Manager->OnOffLeverDoors[IndexInManager].Doors[i]->PlaySound();
						}

					}
					
					//Make the player unable to interact
					OverlappingPlayer->CanInteract = false;
					//Reset the timer
					OpenTimer = 0.0f;
				}
				else //If the lever is in its closed state
				{
					//Set its state to open
					IsOpen = true;

					//Play the lever sound
					if (OpenSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
					}

					if (Manager)
					{
						//Iterate through all the doors
						for (int i = 0; i < Manager->OnOffLeverDoors.Num(); i++)
						{
							//Have the doors play their open/close sounds
							Manager->OnOffLeverDoors[IndexInManager].Doors[i]->PlaySound();
						}

					}

					//Make the player unable to interact
					OverlappingPlayer->CanInteract = false;
					//Reset the timer
					OpenTimer = 0.0f;
				}

			}
		}
		else
		{
			//If the player is unable to interact, make them not interact
			OverlappingPlayer->IsInteracting = false;
		}

	}

	//Update the position of the lever over time based on whether its open or closed
	if (IsOpen)
	{
		MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime));
	}
	else
	{
		MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
	}

	//Increment the time before the player can interact again
	OpenTimer += DeltaTime;

}

void AOnOffLever::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			//If the overlapping actor is a player
			if (playerActor)
			{
				//Set the overlapping player to this player 
				OverlappingPlayer = playerActor;

			}
		}
	}
}

void AOnOffLever::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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