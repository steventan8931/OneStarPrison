// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveStepsButton.h"
#include "PlayerCharacter.h"
#include "InteractiveSteps.h"
#include "InteractiveStepsManager.h"

#include "Components/BoxComponent.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractiveStepsButton::AInteractiveStepsButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 100));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveStepsButton::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractiveStepsButton::OnOverlapEnd);

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	MovableMesh->SetupAttachment(Mesh);

	HoldPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HoldPosition"));
	HoldPosition->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AInteractiveStepsButton::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractiveStepsButton::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveStepsButton, LinkedSteps);
	DOREPLIFETIME(AInteractiveStepsButton, StepsManager);

	DOREPLIFETIME(AInteractiveStepsButton, HandleOpenRotation);
	DOREPLIFETIME(AInteractiveStepsButton, HandleClosedRotation);

	DOREPLIFETIME(AInteractiveStepsButton, OverlappingPlayer);

	DOREPLIFETIME(AInteractiveStepsButton, BoxCollision);


	DOREPLIFETIME(AInteractiveStepsButton, HandleRotateSpeed);
	DOREPLIFETIME(AInteractiveStepsButton, HoldPosition);
	DOREPLIFETIME(AInteractiveStepsButton, HandleStartDelay);
	DOREPLIFETIME(AInteractiveStepsButton, HandleStartTimer);
}

// Called every frame
void AInteractiveStepsButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If there are linked steps for this lever
	if (LinkedSteps.Num() > 0)
	{
		//If there is a pointer to the steps manager
		if (StepsManager)
		{
			if (OverlappingPlayer != nullptr)
			{
				//If the player is interacting
				if (OverlappingPlayer->IsInteracting)
				{
					HandleStartTimer += DeltaTime;

					//Snap to location
					OverlappingPlayer->SetActorLocation(HoldPosition->GetComponentLocation());
					OverlappingPlayer->SetActorRotation(HoldPosition->GetComponentRotation());

					//Move the handle to the open position over time
					if (HandleStartTimer >= HandleStartDelay)
					{
						MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime * HandleRotateSpeed));
					}

					//Tell the steps manager to open these steps
					StepsManager->IsStepOpen = true;
					//Set the current moving steps for the manager to be the this levers linked steps
					StepsManager->SetOpenStep(LinkedSteps);

					//Play the lever sound if it hasn't already been played within the time
					if (!SoundPlayed)
					{
						if (SoundTimer >= SoundPlayDelay)
						{
							if (OnSound)
							{
								UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnSound, GetActorLocation());
							}
							SoundTimer = 0.0f;
						}
						SoundPlayed = true;
					}
					else
					{
						SoundTimer += DeltaTime;
					}

				}
				else //When the player stops interacting (holding down)
				{
					HandleStartTimer = 0;

					//Update the mesh rotation to its closed rotation
					MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
					//Tell the steps manager to close these steps
					StepsManager->IsStepOpen = false;


					//Play the lever sound if it has been played and the lever has been released
					if (SoundPlayed)
					{
						if (SoundTimer >= SoundPlayDelay)
						{
							if (OnSound)
							{
								UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnSound, GetActorLocation());
							}
							SoundTimer = 0.0f;
						}

						SoundPlayed = false;
					}
					else
					{
						SoundTimer += DeltaTime;
					}
				}
			}
			else
			{
				//Update the mesh rotation to its closed rotation
				MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
			}
		}

	}
}

void AInteractiveStepsButton::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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

void AInteractiveStepsButton::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
					OverlappingPlayer->IsInteracting = false;
					OverlappingPlayer = nullptr;
				}
			}
		}
	}
}
