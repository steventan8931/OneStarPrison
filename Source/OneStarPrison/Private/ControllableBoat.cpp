// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllableBoat.h"
#include "Components/BoxComponent.h"
#include "PickupableBook.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AControllableBoat::AControllableBoat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 100));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AControllableBoat::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AControllableBoat::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AControllableBoat::BeginPlay()
{
	Super::BeginPlay();

	cacheTransform = GetActorTransform();
}

void AControllableBoat::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AControllableBoat, IsMoving);
	DOREPLIFETIME(AControllableBoat, ItemInserted);

	DOREPLIFETIME(AControllableBoat, OverlappingPlayer);
	DOREPLIFETIME(AControllableBoat, OverlappingPlayer2);
}

// Called every frame
void AControllableBoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If both players are overlapping
	if (OverlappingPlayer && OverlappingPlayer2)
	{
		//If either player dies
		if (OverlappingPlayer->IsDead || OverlappingPlayer2->IsDead)
		{
			//Reset the boat to its starting position
			SetActorTransform(cacheTransform);

			//Make the dead player refernce to null and stop the boat from moving
			if (OverlappingPlayer->IsDead)
			{
				OverlappingPlayer = nullptr;
				IsMoving = false;
			}

			if (OverlappingPlayer2->IsDead)
			{
				OverlappingPlayer2 = nullptr;
				IsMoving = false;
			}
			return;
		}

		//If the boat is moving, updates it position, making it go forward in its forward vector
		if (IsMoving)
		{
			FVector newPos = FMath::Lerp(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * Speed), DeltaTime);
			SetActorLocation(newPos);
			return;
		}

		//Check for players item and interaction
		CheckItem(OverlappingPlayer);
		CheckItem(OverlappingPlayer2);
	}
}

void AControllableBoat::CheckItem_Implementation(APlayerCharacter* _Player)
{
	if (_Player)
	{
		if (_Player->PickedUpItem)
		{
			APickupableBook* book = Cast<APickupableBook>(_Player->PickedUpItem);

			//If the held item is a OBJECT
			if (book)
			{
				//Allow the player to interact
				_Player->CanInteract = true;

				//If the player clicked interact
				if (_Player->IsInteracting)
				{
					//Attach the OBJECT to this actor
					book->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

					//Play the insert sound
					if (InsertSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), InsertSound, GetActorLocation());
					}

					//Make the player unable to continue interacting
					_Player->CanInteract = false;
					_Player->IsInteracting = false;
					//Remove the item from the player
					_Player->PickedUpItem = nullptr;

					//Set the boat to move
					IsMoving = true;
					//Set item inserted to true
					ItemInserted = true;
				}
			}
		}
	}
}


void AControllableBoat::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Only check if the boat isn't already moving
	if (IsMoving)
	{
		return;
	}

	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			if (ItemInserted)
			{
				IsMoving = true;
				//return;
			}

			//If there isn't already an overlapping player
			if (!OverlappingPlayer)
			{
				//Set the current player to the overlapping player
				OverlappingPlayer = playerActor;
			}
			else
			{
				if (playerActor != OverlappingPlayer)
				{
					//Set current player to the second player
					OverlappingPlayer2 = playerActor;
				}
			}
		}

	}
}

void AControllableBoat::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Only check if the boat isn't already moving
	if (IsMoving)
	{
		return;
	}

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

					if (OverlappingPlayer2)
					{
						OverlappingPlayer2->CanInteract = false;
					}
				}
				else
				{
					//Otherwise make the overlapping actor unable to interact
					playerActor->CanInteract = false;
				}
			}

			//If an overlapping player exists
			if (OverlappingPlayer2)
			{
				//If the current player is the overlapping player
				if (playerActor == OverlappingPlayer2)
				{
					//Make the overlapping player can't interact 
					OverlappingPlayer2->CanInteract = false;
					OverlappingPlayer2 = nullptr;

					if (OverlappingPlayer)
					{
						OverlappingPlayer->CanInteract = false;
					}
				}
				else
				{
					//Otherwise make the overlapping actor unable to interact
					playerActor->CanInteract = false;
				}
			}
		}
	}
}

