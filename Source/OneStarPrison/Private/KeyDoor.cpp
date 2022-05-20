// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyDoor.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "PickupableKey.h"

#include "Blueprint/UserWidget.h"
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
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(200,200,200), FColor::Green, true);
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
		if (OverlappingPlayer->IsInteracting)
		{
			APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer->PickedUpItem);
			if (key)
			{
				if (IsKeyOneTimeUse)
				{
					OverlappingPlayer->PickedUpItem->Destroy();
					OverlappingPlayer->PickedUpItem = nullptr;
				}

				IsOpen = true;

				if (CurrentWidget)
				{
					CurrentWidget->RemoveFromParent();
				}

				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Open"));
			}
		}
	}
}

void AKeyDoor::OpenDoor(float _DeltaTime)
{
	if (GetActorLocation() != OpenPosition)
	{
		FVector newPos = FMath::Lerp(GetActorLocation(), OpenPosition, _DeltaTime);
		SetActorLocation(newPos);
	}
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
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Can Interact"));
				OverlappingPlayer->CanInteract = true;

				APickupableKey* key = Cast<APickupableKey>(OverlappingPlayer->PickedUpItem);
				if (key)
				{
					if (!IsOpen)
					{
						InteractPopUp();
					}

				}
			}
		}
	}
}

void AKeyDoor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{

		if (OverlappingPlayer != nullptr)
		{
			if (CurrentWidget)
			{
				CurrentWidget->RemoveFromParent();
			}

			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Player left"));
			OverlappingPlayer = nullptr;
		}

	}
}

void AKeyDoor::InteractPopUp()
{
	if (HUDWidgetClass != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("WIDGET CLASS EXIST"));

		CurrentWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), OverlappingPlayer->PlayerIndex), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToPlayerScreen();
		}
	}
}