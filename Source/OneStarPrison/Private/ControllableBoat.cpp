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
}

// Called every frame
void AControllableBoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	//If there is an overlapping player
	if (OverlappingPlayer)
	{
		if (OverlappingPlayer->IsDead)
		{
			SetActorTransform(cacheTransform);
		}

		if (IsMoving)
		{
			FVector newPos = FMath::Lerp(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * Speed), DeltaTime);
			SetActorLocation(newPos);
			return;
		}
		//If they have an item
		if (OverlappingPlayer->PickedUpItem)
		{
			APickupableBook* book = Cast<APickupableBook>(OverlappingPlayer->PickedUpItem);

			//If the held item is a book
			if (book)
			{
				//Allow the player to interact
				OverlappingPlayer->CanInteract = true;

				//If the player clicked interact
				if (OverlappingPlayer->IsInteracting)
				{
					//Attach the book to this actor
					book->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

					//Play the insert sound
					if (InsertSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), InsertSound, GetActorLocation());
					}

					//Make the player unable to continue interacting
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->IsInteracting = false;
					//Remove the item from the player
					OverlappingPlayer->PickedUpItem = nullptr;

					//Set the boat to move
					IsMoving = true;
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
			//If there isn't already an overlapping player
			if (!OverlappingPlayer)
			{
				//Set the current player to the overlapping player
				OverlappingPlayer = playerActor;
			}
			else
			{
				//If there is already an overlapping player
				//But the current actor has a item
				if (playerActor->PickedUpItem)
				{
					APickupableBook* book = Cast<APickupableBook>(playerActor->PickedUpItem);
					//And the item is a book
					if (book)
					{
						//Allow this player to interact 
						playerActor->CanInteract = true;
						//Make the previous overlapping player unable to interact and make overlapping player the current player with the book
						OverlappingPlayer->CanInteract = false;
						OverlappingPlayer = playerActor;
					}
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

