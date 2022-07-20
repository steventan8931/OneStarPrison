// Fill out your copyright notice in the Description page of Project Settings.


#include "SecretBookshelf.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

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

	DOREPLIFETIME(ASecretBookshelf, OpenPosition);
}

// Called every frame
void ASecretBookshelf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckInsertedBook())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Hello s")));

		if (Mesh->GetComponentLocation() != OpenPosition)
		{
			FVector newPos = FMath::Lerp(Mesh->GetComponentLocation(), OpenPosition, DeltaTime);
			Mesh->SetWorldLocation(newPos);
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
				if (OverlappingPlayer->IsInteracting)
				{
					switch (book->Code)
					{
					case 0:
						Book1Inserted = true;
						book->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						book->SetActorLocation(Book1Position->GetComponentLocation());
						OverlappingPlayer->CanInteract = false;
						OverlappingPlayer->PickedUpItem = nullptr;
						break;
					case 1:
						Book2Inserted = true;
						book->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						book->SetActorLocation(Book2Position->GetComponentLocation());
						OverlappingPlayer->CanInteract = false;
						OverlappingPlayer->PickedUpItem = nullptr;
						break;
					}

					if (CheckInsertedBook())
					{
						if (CompleteSound)
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), CompleteSound, GetActorLocation());
						}
					}

					OverlappingPlayer->IsInteracting = false;
				}

			}
		}
	}
}

bool ASecretBookshelf::CheckInsertedBook()
{
	if (!Book1Inserted)
	{
		return false;
	}

	if (!Book2Inserted)
	{
		return false;
	}

	return true;
}

void ASecretBookshelf::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				OverlappingPlayer = playerActor;
				if (playerActor->PickedUpItem)
				{
					APickupableBook* book = Cast<APickupableBook>(playerActor->PickedUpItem);

					if (book)
					{
						OverlappingPlayer->CanInteract = true;
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
			}
		}
	}
}