// Fill out your copyright notice in the Description page of Project Settings.


#include "SecretBookshelf.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
ASecretBookshelf::ASecretBookshelf()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Book1Position = CreateDefaultSubobject<USceneComponent>(TEXT("Book1Position"));
	Book1Position->SetupAttachment(Mesh);

	Book2Position = CreateDefaultSubobject<USceneComponent>(TEXT("Book2Position"));
	Book2Position->SetupAttachment(Mesh);

	Book3Position = CreateDefaultSubobject<USceneComponent>(TEXT("Book3Position"));
	Book3Position->SetupAttachment(Mesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASecretBookshelf::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ASecretBookshelf::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ASecretBookshelf::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASecretBookshelf::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASecretBookshelf, OpenRotation);

	DOREPLIFETIME(ASecretBookshelf, Book1Inserted);
	DOREPLIFETIME(ASecretBookshelf, Book2Inserted);
	DOREPLIFETIME(ASecretBookshelf, Book3Inserted);
	
	DOREPLIFETIME(ASecretBookshelf, AllBooksInserted);

	DOREPLIFETIME(ASecretBookshelf, CompleteSound);
	DOREPLIFETIME(ASecretBookshelf, OverlappingPlayer);
	
}

// Called every frame
void ASecretBookshelf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If all books are inserted
	if (AllBooksInserted)
	{
		//Update the position of the bookshelf to its open rotation
		if (Mesh->GetComponentRotation() != OpenRotation)
		{
			FRotator newRot = FMath::Lerp(Mesh->GetComponentRotation(), OpenRotation, DeltaTime * OpenSpeed);
			Mesh->SetWorldRotation(newRot);
		}

		//Don't do the code after
		return;
	}

	//If there is an overlapping player
	if (OverlappingPlayer)
	{
		//If they have an item
		if (OverlappingPlayer->PickedUpItem)
		{
			APickupableBook* book = Cast<APickupableBook>(OverlappingPlayer->PickedUpItem);

			//If the held item is a book
			if (book)
			{
				//Change the players interact type to insert
				OverlappingPlayer->InteractType = EInteractType::Insert;

				//Allow the player to interact
				OverlappingPlayer->CanInteract = true;

				//If the player clicked interact
				if (OverlappingPlayer->IsInteracting)
				{
					//Attach the book to this actor
					book->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

					//Place the book in the correct position based on the code
					switch (book->Code)
					{
					case 0:
						Book1Inserted = true;
						book->SetActorLocation(Book1Position->GetComponentLocation());
						break;
					case 1:
						Book2Inserted = true;
						book->SetActorLocation(Book2Position->GetComponentLocation());
						break;
					case 2:
						Book3Inserted = true;
						book->SetActorLocation(Book3Position->GetComponentLocation());
						break;
					}

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

				}

			}
		}
	}

	CheckInsertedBook();


}

void ASecretBookshelf::CheckInsertedBook_Implementation()
{
	//Checks for if each book has been inserted
	//If they aren't they set all books inserted to false and stop checking
	if (!Book1Inserted)
	{
		AllBooksInserted = false;
		return;
	}

	if (!Book2Inserted)
	{
		AllBooksInserted = false;
		return;
	}

	if (!Book3Inserted)
	{
		AllBooksInserted = false;
		return;
	}

	//Make it start moving the bookshelf
	AllBooksInserted = true;

	if (AllBooksInserted)
	{
		//If all each book inserted managed to all be true
		//Play the complete sound of inserting all the books
		if (CompleteSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), CompleteSound, GetActorLocation());
		}

	}
}

void ASecretBookshelf::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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

void ASecretBookshelf::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
				else
				{
					//Otherwise make the overlapping actor unable to interact
					playerActor->CanInteract = false;
				}
			}
		}
	}
}