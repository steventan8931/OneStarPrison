 // Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "CrankliftPlatform.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
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

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	MovableMesh->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (MovingSound)
	{
		if (Platform)
		{
			AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, MovingSound, Platform->GetActorLocation());
			if (AudioComponent)
			{
				//AudioComponent->SetIsReplicated(true);
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("moving"));
			}

		}
	}
	ServerPlaySound(false);
}

void AMovingPlatform::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMovingPlatform, HandleOpenRotation);
	DOREPLIFETIME(AMovingPlatform, HandleClosedRotation);
	DOREPLIFETIME(AMovingPlatform, Platform);
	DOREPLIFETIME(AMovingPlatform, OpenPosition);
	DOREPLIFETIME(AMovingPlatform, ClosedPosition);

	DOREPLIFETIME(AMovingPlatform, IsMoving);
	DOREPLIFETIME(AMovingPlatform, MoveSpeed);
	
		
	//DOREPLIFETIME(AMovingPlatform, AudioComponent);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Platform)
	{
		ServerPlaySound(false);

		if (OverlappingPlayer != nullptr)
		{
			if (OverlappingPlayer->IsInteracting)
			{
				if (AudioComponent)
				{
					if(!AudioComponent->IsPlaying())
					{
						AudioComponent->Play();
						ServerPlaySound(false);
					}
				}

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
				MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime));
				FVector newPos = FMath::Lerp(Platform->GetActorLocation(), OpenPosition, DeltaTime * MoveSpeed);
				Platform->SetActorLocation(newPos);


				if (FVector::Distance(Platform->GetActorLocation(), OpenPosition) < 500)
				{
					ServerPlaySound(true);
				}
				else
				{
					ServerPlaySound(false);
				}
			}



		}
		else
		{
			if (Platform->GetActorLocation() != ClosedPosition)
			{
				MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
				FVector newPos = FMath::Lerp(Platform->GetActorLocation(), ClosedPosition, DeltaTime * MoveSpeed);
				Platform->SetActorLocation(newPos);

				if (FVector::Distance(Platform->GetActorLocation(), ClosedPosition) < 500)
				{
					ServerPlaySound(true);
				}
				else
				{
					ServerPlaySound(false);

				}
			}
			else
			{
				ServerPlaySound(false);
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

void AMovingPlatform::ServerPlaySound_Implementation(bool _IsPaused)
{
	ClientPlaySound(_IsPaused);
}

void AMovingPlatform::ClientPlaySound_Implementation(bool _IsPaused)
{
	if (AudioComponent)
	{
		AudioComponent->SetPaused(_IsPaused);
	}
}