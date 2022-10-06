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
			//Set up audio component to have utilise the moving sound
			AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, MovingSound, Platform->GetActorLocation());

			if (AudioComponent)
			{
				ServerPlaySound(true);
				AudioComponent->SetPaused(true);
			}

			//cache for fail movement
			cacheOpenPos = Platform->GetActorLocation() + FVector(0, 0, 50.0f);
		}
	}



}

void AMovingPlatform::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMovingPlatform, HandleOpenRotation);
	DOREPLIFETIME(AMovingPlatform, HandleClosedRotation);
	DOREPLIFETIME(AMovingPlatform, Platform);
	DOREPLIFETIME(AMovingPlatform, OpenPosition);
	DOREPLIFETIME(AMovingPlatform, ClosedPosition);

	DOREPLIFETIME(AMovingPlatform, IsOpen);
	DOREPLIFETIME(AMovingPlatform, MoveSpeed);
	
	DOREPLIFETIME(AMovingPlatform, AudioComponent);
	DOREPLIFETIME(AMovingPlatform, cacheInteract);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If theree is a platform linked
	if (Platform)
	{
		//If there is a player
		if (OverlappingPlayer != nullptr)
		{
			//If there is a player holding interacting
			if (OverlappingPlayer->IsInteracting)
			{
				if (AudioComponent)
				{
					if(!AudioComponent->IsPlaying())
					{
						ServerPlaySound(false);
					}
				}

				//Move the platform to its open position
				IsOpen = true;

				//If the player is interacting and the platform is unable to move
				if (!Platform->CanMove)
				{
					if (cacheInteract == false)
					{
						//Play the return sound
						if (FailSound)
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), FailSound, GetActorLocation());
						}
						cacheInteract = true;
					}

				}
			}
			else
			{
				cacheInteract = false;
				//Move the platform to its closed position
				IsOpen = false;
			}
		}
		else
		{
			//Move the platform to its closed position
			IsOpen = false;
		}


		//If the lever is pulled held
		if (IsOpen)
		{
			if (Platform->CanMove)
			{
				//Move the platform to its open position over time
				if (Platform->GetActorLocation() != OpenPosition)
				{
					//Update the handle to its open rotation over time
					MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime));
					FVector newPos = FMath::Lerp(Platform->GetActorLocation(), OpenPosition, DeltaTime * MoveSpeed);
					Platform->SetActorLocation(newPos);

					//If the platform is almost at its open position stop playing the audio
					if (FVector::Distance(Platform->GetActorLocation(), OpenPosition) < 100)
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
					//If the platform is at its open position stop playing the audio
					ServerPlaySound(true);
				}
			}
			else
			{
				//Move the platform to its open position over time
				if (Platform->GetActorLocation() != cacheOpenPos)
				{
					//Update the handle to its open rotation over time
					MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime));
					FVector newPos = FMath::Lerp(Platform->GetActorLocation(), cacheOpenPos, DeltaTime * MoveSpeed * 2);
					Platform->SetActorLocation(newPos);
				}
			}
		}
		else
		{
			//Move the platform to its closed position over time
			if (Platform->GetActorLocation() != ClosedPosition)
			{
				//Update the handle to its closed rotation over time
				MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
				FVector newPos = FMath::Lerp(Platform->GetActorLocation(), ClosedPosition, DeltaTime * MoveSpeed);
				Platform->SetActorLocation(newPos);

				//If the platform is almost at its closed position stop playing the audio
				if (FVector::Distance(Platform->GetActorLocation(), ClosedPosition) < 100)
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
				//If the platform is at its closed position stop playing the audio
				ServerPlaySound(true);
			}

		}

		//Cache to not play sound on spawn
		if (FirstFrame)
		{
			ServerPlaySound(true);
			FirstFrame = false;
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

			//If the overlapping actor is a player
			if (playerActor)
			{
				//Set the overlapping player to this player and allow them to interact
				OverlappingPlayer = playerActor;
				OverlappingPlayer->CanInteract = true;
				//Change the players interact type to lever pulling
				OverlappingPlayer->InteractType = EInteractType::LeverPull;
			}
		}
	}
}

void AMovingPlatform::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If the overlapping player exists
			if (OverlappingPlayer)
			{
				//If this overlapping player is the overlapping player
				if (playerActor == OverlappingPlayer)
				{
					//Remove the players ability to interact with the lever
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer = nullptr;

					//Sets the platform to move back
					IsOpen = false;
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