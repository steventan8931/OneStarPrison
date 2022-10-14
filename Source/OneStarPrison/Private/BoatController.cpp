// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatController.h"
#include "Components/BoxComponent.h"
#include "PickupableBook.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

#include "ControllableBoat.h"

// Sets default values
ABoatController::ABoatController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("Trigger"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 100));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoatController::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABoatController::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ABoatController::BeginPlay()
{
	Super::BeginPlay();

	StartingRotation = GetActorRotation();
}

void ABoatController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoatController, InteractDelay);
	DOREPLIFETIME(ABoatController, InteractTimer);

	DOREPLIFETIME(ABoatController, RotationScale);

	DOREPLIFETIME(ABoatController, IsAnchor);
	DOREPLIFETIME(ABoatController, AnchorRemoved);

	DOREPLIFETIME(ABoatController, IsRowingRight);
	
}

// Called every frame
void ABoatController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If  the boat is connected
	if (Boat)
	{
		//If the boat isnt moving dont check for player interaction
		if (!Boat->IsMoving)
		{
			//Whether the the controller is an anchor
			if (IsAnchor)
			{
				//If the player is overlapping
				if (OverlappingPlayer)
				{
					//Whether the boat has the item inserted
					if (!Boat->ItemInserted)
					{
						//Allow the player to interact
						OverlappingPlayer->CanInteract = true;

						//If the player is interacting
						if (OverlappingPlayer->IsInteracting)
						{
							//Unanchors the boat
							Boat->InsertItem();

							//Set anchor removed to true 
							AnchorRemoved = true;

							//Stop player from furthering interacting
							OverlappingPlayer->CanInteract = false;

							OverlappingPlayer->IsInteracting = false;
							return;
						}
					}
				}
			}
			return;
		}
		else
		{
			//Dont move the boat if this is an anchor
			if (IsAnchor)
			{
				return;
			}

			InteractTimer += DeltaTime;

			//If the player is overlapping
			if (OverlappingPlayer)
			{
				//If the player's last interact has been longer than the interact delay
				if (InteractTimer >= InteractDelay)
				{
					SetActorRotation(StartingRotation);
					//Allow the player to interact
					OverlappingPlayer->CanInteract = true;
					if (OverlappingPlayer)
					{ 
						if (IsRowingRight)
						{
							OverlappingPlayer->InteractType = EInteractType::RowRight;
						}
						else
						{
							OverlappingPlayer->InteractType = EInteractType::RowLeft;
						}
					}


					//If the player is interacting
					if (OverlappingPlayer->IsInteracting)
					{
						//Update the rotation of the boat
						if (HasAuthority())
						{
							FRotator rot = FRotator(0, RotationScale, 0);
							FRotator newRot = FMath::Lerp(Boat->GetActorRotation(), Boat->GetActorRotation() + rot, DeltaTime * 2);
							Boat->SetActorRotation(newRot);
						}

						//If the player is overlapping
						if (OverlappingPlayer)
						{
							if (OverlappingPlayer->CanMove)
							{
								if (IsRowingRight)
								{
									OverlappingPlayer->SetActorLocation(Boat->RightRowPosition->GetComponentLocation());
									OverlappingPlayer->SetActorRotation(Boat->RightRowPosition->GetComponentRotation());
								}
								else
								{
									OverlappingPlayer->SetActorLocation(Boat->LeftRowPosition->GetComponentLocation());
									OverlappingPlayer->SetActorRotation(Boat->LeftRowPosition->GetComponentRotation());
								}
							}

						}

						OverlappingPlayer->CanInteract = false;
						InteractTimer = 0.0f;
						return;
					}

				}
				else
				{
					//Don't allow the overlapping player to interact
					//OverlappingPlayer->IsInteracting = false;
					OverlappingPlayer->CanInteract = false;
				}
			}
			else
			{
				SetActorRotation(StartingRotation);
			}
		}
	}

}

void ABoatController::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If this is an anchor and the player already interacted
	if (IsAnchor && Boat && Boat->ItemInserted)
	{
		return;
	}

	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If there isn't already an overlapping player
			if (!OverlappingPlayer)
			{
				//Set the current player to the overlapping player
				OverlappingPlayer = playerActor;
			}
		}

	}
}

void ABoatController::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If an overlapping player exists
			if (OverlappingPlayer)
			{
				//If the current player is the overlapping player
				if (playerActor == OverlappingPlayer)
				{
					//Make the overlapping player can't interact 
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer = nullptr;
				}
			}
		}
	}
}

