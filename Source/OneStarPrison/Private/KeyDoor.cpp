// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyDoor.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "PickupableKey.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AKeyDoor::AKeyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	RootComponent = BoxCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKeyDoor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AKeyDoor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AKeyDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AKeyDoor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKeyDoor, OpenPosition);
}

// Called every frame
void AKeyDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		OpenDoor(DeltaTime);
		return;
	}

	if (OverlappingPlayer)
	{
		APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer->PickedUpItem);
		if (key)
		{
			if (key->KeyCode == KeyCode)
			{
				OverlappingPlayer->CanInteract = true;
			}

			if (OverlappingPlayer->IsInteracting)
			{
				if (IsKeyOneTimeUse)
				{
					OverlappingPlayer->PickedUpItem->Destroy();
					OverlappingPlayer->PickedUpItem = nullptr;
				}

				if (OpenSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
				}

				IsOpen = true;

				OverlappingPlayer->CanInteract = false;

				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Open"));
			
			}
		}

	}
}

void AKeyDoor::OpenDoor(float _DeltaTime)
{
	if (Mesh->GetComponentLocation() != OpenPosition)
	{
		FVector newPos = FMath::Lerp(Mesh->GetComponentLocation(), OpenPosition, _DeltaTime);
		Mesh->SetWorldLocation(newPos);
	}
	//if (GetActorLocation() != OpenPosition)
	//{
	//	FVector newPos = FMath::Lerp(GetActorLocation(), OpenPosition, _DeltaTime);
	//	SetActorLocation(newPos);
	//}
}

void AKeyDoor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				OverlappingPlayer = playerActor;
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Can Interact"));
				
			}
		}
	}
}

void AKeyDoor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		if (playerActor)
		{
			if (OverlappingPlayer != nullptr)
			{
				OverlappingPlayer->CanInteract = false;
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Player left"));
				OverlappingPlayer = nullptr;
			}
		}
	}
}

