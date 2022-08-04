// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MazePawn.h"
// Sets default values
APossessTrigger::APossessTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 100));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APossessTrigger::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APossessTrigger::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APossessTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APossessTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingPlayer)
	{
		if (OverlappingPlayer->IsInteracting)
		{
			APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			controller->Possess(Pawn);

			AMazePawn* mazepawn = Cast<AMazePawn>(Pawn);

			if (mazepawn)
			{
				mazepawn->CurrentPlayer = OverlappingPlayer;
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, TEXT("player set s"));
			}
			OverlappingPlayer->SetVeloctiy(FVector::ZeroVector);
			OverlappingPlayer->IsInteracting = false;
			//OverlappingPlayer->GetCharacterMovement()->LinearVelocity = FVector(0,0,0);
		}
	}
}

void APossessTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				OverlappingPlayer = playerActor;
				OverlappingPlayer->CanInteract = true;
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, TEXT("Hello s"));

			}
		}
	}
}

void APossessTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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