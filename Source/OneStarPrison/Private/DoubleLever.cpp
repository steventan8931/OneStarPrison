// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleLever.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
ADoubleLever::ADoubleLever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoubleLever::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ADoubleLever::OnOverlapEnd);

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	MovableMesh->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ADoubleLever::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoubleLever::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoubleLever, HandleOpenRotation);
	DOREPLIFETIME(ADoubleLever, HandleClosedRotation);

}

// Called every frame
void ADoubleLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//If the lever has been open for longer than its designated time
	if (OpenTimer >= OpenTime)
	{
		if (OpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
		}
		//Close the lever and reset the timer
		IsOpen = false;
		OpenTimer = 0.0f;
	}

	//If there is an overlapping player
	if (OverlappingPlayer != nullptr)
	{
		//If the overlapping player pressed interact
		if (OverlappingPlayer->IsInteracting)
		{
			//and the lever isn't already opened
			if (!IsOpen)
			{
				//Set the lever to its open state
				IsOpen = true;
				if (OpenSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
				}
				//Stop the player from interacting
				OverlappingPlayer->IsInteracting = false;
			}

		}
	}

	//Update the position of the handle based on whether its open or closed
	if (IsOpen)
	{
		//If it is open increment the open timer over time, so it can reset back to closed after a delay
		OpenTimer += DeltaTime;
		MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleOpenRotation, DeltaTime));
	}
	else
	{
		MovableMesh->SetRelativeRotation(FMath::Lerp(MovableMesh->GetRelativeRotation(), HandleClosedRotation, DeltaTime));
	}
}

void ADoubleLever::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ADoubleLever::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
				}
			}
		}
	}
}