 // Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "CrankliftPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatform::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatform::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Platform)
	{
		if (OverlappingPlayer != nullptr)
		{
			if (OverlappingPlayer->IsInteracting)
			{
				IsMoving = true;
			}
			else
			{
				IsMoving = false;
			}
		}
		else
		{
			IsMoving = false;
		}


		if (IsMoving)
		{
			if (Platform->GetActorLocation() != OpenPosition)
			{
				FVector newPos = FMath::Lerp(Platform->GetActorLocation(), OpenPosition, DeltaTime * MoveSpeed);
				Platform->SetActorLocation(newPos);
			}
		}
		else
		{
			if (Platform->GetActorLocation() != ClosedPosition)
			{
				FVector newPos = FMath::Lerp(Platform->GetActorLocation(), ClosedPosition, DeltaTime * MoveSpeed);
				Platform->SetActorLocation(newPos);
			}
		}

	}
}

void AMovingPlatform::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				OverlappingPlayer = playerActor;
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, playerActor->GetName());
				OverlappingPlayer->CanInteract = true;

			}
		}
	}
}

void AMovingPlatform::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
					IsMoving = false;
					OverlappingPlayer = nullptr;
				}
			}
		}
	}
}

