// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingSteps.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AFloatingSteps::AFloatingSteps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(100, 100, 100));
	RootComponent = BoxCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFloatingSteps::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AFloatingSteps::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AFloatingSteps::BeginPlay()
{
	Super::BeginPlay();
	
	UpHeight = GetActorLocation().Z;
}

void AFloatingSteps::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFloatingSteps, BoxCollision);

	DOREPLIFETIME(AFloatingSteps, UpHeight);
	DOREPLIFETIME(AFloatingSteps, DownHeight);
	DOREPLIFETIME(AFloatingSteps, IsPlayerColliding);
	DOREPLIFETIME(AFloatingSteps, MoveSpeed);
}

// Called every frame
void AFloatingSteps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPlayerColliding)
	{
		//If the object has not reached its minimum height
		if (GetActorLocation().Z >= DownHeight)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("moving down"));
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - DeltaTime * MoveSpeed));

		}
	}
	else
	{
		//If the object has not reached its maximum height
		if (GetActorLocation().Z <= UpHeight)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("moving up"));
			//Keep moving the platform up over time
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + DeltaTime * MoveSpeed));

		}
	}

}

void AFloatingSteps::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

		//If the overlapping actor is a player
		if (playerActor)
		{
			//If overlapping player is null, set the overlapping player to the current player
			if (!OverlappingPlayer)
			{
				//Set the overlapping player to the overlapping actor
				OverlappingPlayer = playerActor;
				//Set Player Colliding to true
				IsPlayerColliding = true;
			}
		}
	}
}

void AFloatingSteps::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
					//Set Player colliding to true and set overlapping player to null
					IsPlayerColliding = false;
					OverlappingPlayer = nullptr;
				}
			}
		}
	}


}

