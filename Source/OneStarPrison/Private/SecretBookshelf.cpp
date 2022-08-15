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
	DOREPLIFETIME(ASecretBookshelf, BothBooksInserted);
}

// Called every frame
void ASecretBookshelf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckInsertedBook();

	if (BothBooksInserted)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Hello s")));
		if (!SoundPlayed)
		{
			if (CompleteSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), CompleteSound, GetActorLocation());
			}
			SoundPlayed = true;
		}

		if (Mesh->GetComponentRotation() != OpenRotation)
		{
			FRotator newRot = FMath::Lerp(Mesh->GetComponentRotation(), OpenRotation, DeltaTime);
			Mesh->SetWorldRotation(newRot);
		}

		return;
	}


	if (OverlappingPlayer)
	{
		if (OverlappingPlayer->PickedUpItem)
		{
			APickupableBook* book = Cast<APickupableBook>(OverlappingPlayer->PickedUpItem);

			if (book)
			{
				OverlappingPlayer->CanInteract = true;
				if (OverlappingPlayer->IsInteracting)
				{
					book->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

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

					if (InsertSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), InsertSound, GetActorLocation());
					}

					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->PickedUpItem = nullptr;
					OverlappingPlayer->IsInteracting = false;
				}

			}
		}
	}


}

void ASecretBookshelf::CheckInsertedBook_Implementation()
{
	RPCCheckInsertedBook();
}

void ASecretBookshelf::RPCCheckInsertedBook_Implementation()
{
	if (!Book1Inserted)
	{
		BothBooksInserted = false;
		return;
	}

	if (!Book2Inserted)
	{
		BothBooksInserted = false;
		return;
	}

	if (!Book3Inserted)
	{
		BothBooksInserted = false;
		return;
	}

	BothBooksInserted = true;
}

void ASecretBookshelf::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		if (playerActor)
		{
			if (OverlappingPlayer == nullptr)
			{
				OverlappingPlayer = playerActor;
			}
			else
			{
				if (playerActor->PickedUpItem)
				{
					APickupableBook* book = Cast<APickupableBook>(playerActor->PickedUpItem);

					if (book)
					{
						playerActor->CanInteract = true;
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

		if (playerActor)
		{
			if (OverlappingPlayer != nullptr)
			{
				if (playerActor == OverlappingPlayer)
				{
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer = nullptr;
				}
				else
				{
					playerActor->CanInteract = false;
				}
			}
		}
	}
}