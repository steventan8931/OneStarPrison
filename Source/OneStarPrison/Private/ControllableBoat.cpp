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

	LeftRowPosition = CreateDefaultSubobject<USceneComponent>(TEXT("LeftRowPosition"));
	LeftRowPosition->SetupAttachment(RootComponent);
	RightRowPosition = CreateDefaultSubobject<USceneComponent>(TEXT("RightRowPosition"));
	RightRowPosition->SetupAttachment(RootComponent);

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

			//Kill both players if one dies
			OverlappingPlayer->IsDead = true;;
			OverlappingPlayer2->IsDead = true;;

			//Reset interacting
			OverlappingPlayer->IsInteracting = false;
			OverlappingPlayer2->IsInteracting = false;

			//Make the player refernce to null and stop the boat from moving
			OverlappingPlayer = nullptr;
			OverlappingPlayer2 = nullptr;
			IsMoving = false;
			return;
		}

		//If the boat is moving, updates it position, making it go forward in its forward vector
		if (IsMoving)
		{
			FVector newPos = FMath::Lerp(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * Speed), DeltaTime);
			SetActorLocation(newPos);

			if (HasAuthority())
			{
				//Move the object side to side
				FVector loc = FVector(0, 0, cos(GetGameTimeSinceCreation()) * 0.5f);
				Mesh->AddRelativeLocation(loc);
			}

			return;
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
				if (playerActor != OverlappingPlayer)
				{
					//Set current player to the second player
					OverlappingPlayer2 = playerActor;
				}
			}

			//If both players are overlapping and the item has been inserted
			if (OverlappingPlayer && OverlappingPlayer2)
			{
				if (ItemInserted)
				{
					IsMoving = true;
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
					OverlappingPlayer = nullptr;
				}
			}

			//If an overlapping player exists
			if (OverlappingPlayer2)
			{
				//If the current player is the overlapping player
				if (playerActor == OverlappingPlayer2)
				{
					OverlappingPlayer2 = nullptr;
				}
			}
		}
	}
}

void AControllableBoat::InsertItem_Implementation()
{
	//Play the insert sound
	if (InsertSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), InsertSound, GetActorLocation());
	}

	//Set inserted to true
	ItemInserted = true;
}