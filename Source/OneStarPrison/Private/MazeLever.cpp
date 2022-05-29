// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeLever.h"

// Sets default values
AMazeLever::AMazeLever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMazeLever::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMazeLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ACrankliftTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor && (OtherActor != this))
//	{
//		if (OverlappingPlayer == nullptr)
//		{
//			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
//
//			if (playerActor)
//			{
//				OverlappingPlayer = playerActor;
//				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, playerActor->GetName());
//				OverlappingPlayer->CanInteract = true;
//				InteractPopUp();
//			}
//		}
//	}
//}
//
//void ACrankliftTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (OtherActor && (OtherActor != this))
//	{
//		if (OverlappingPlayer != nullptr)
//		{
//			if (CurrentWidget)
//			{
//				CurrentWidget->RemoveFromParent();
//			}
//
//			IsMovingUp = false;
//			OverlappingPlayer = nullptr;
//		}
//
//	}
//}
//
//void ACrankliftTrigger::InteractPopUp()
//{
//	if (HUDWidgetClass != nullptr)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::White, TEXT("WIDGET CLASS EXIST"));
//
//		CurrentWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), OverlappingPlayer->PlayerIndex), HUDWidgetClass);
//
//		if (CurrentWidget)
//		{
//			CurrentWidget->AddToPlayerScreen();
//		}
//	}
//}