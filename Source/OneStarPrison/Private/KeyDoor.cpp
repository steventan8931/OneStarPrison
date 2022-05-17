// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyDoor.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "PickupableKey.h"

// Sets default values
AKeyDoor::AKeyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKeyDoor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AKeyDoor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AKeyDoor::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(200,200,200), FColor::Green, true);
}

// Called every frame
void AKeyDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		Destroy();
	}
}

void AKeyDoor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		if (playerActor)
		{
			APickupableKey* key = Cast<APickupableKey>(playerActor->PickedUpItem);
			if (key)
			{
				playerActor->PickedUpItem->Destroy();
				playerActor->PickedUpItem = nullptr;
				IsOpen = true;
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Open"));
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
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Can Interact"));
		}


	}
}
