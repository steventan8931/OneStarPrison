// Fill out your copyright notice in the Description page of Project Settings.


#include "Mannequin.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	HelmetPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HelmetPosition"));
	HelmetPosition->SetupAttachment(Mesh);

	ArmorPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ArmorPosition"));
	ArmorPosition->SetupAttachment(Mesh);

	FootwearPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FootwearPosition"));
	FootwearPosition->SetupAttachment(Mesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200, 200, 200));
	BoxCollision->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMannequin::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AMannequin::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMannequin::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMannequin, CorrectArmor);
	DOREPLIFETIME(AMannequin, MannequinEquiped);
}


// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (OverlappingPlayer)
	{
		CheckArmorEquipped();

		if (MannequinEquiped)
		{
			CheckCorrectArmor();

			if (!CorrectArmor)
			{
				if (OverlappingPlayer->IsInteracting)
				{
					for (int i = 0; i < EquippedArray.Num(); i++)
					{
						EquippedArray[i]->Mesh->SetCollisionProfileName("IgnoreOnlyPawn");
						EquippedArray[i]->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
						EquippedArray[i]->Launch(FVector(3, 3, 3));
						EquippedArray[i]->Player = nullptr;
					}

					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Hello s")));

					if (EquipSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
					}

					EquippedArray.Empty();
					EquippedArmor.HelmetEquipped = false;
					EquippedArmor.ArmorEquipped = false;
					EquippedArmor.FootwearEquipped = false;
				}
			}

		}

		if (OverlappingPlayer->PickedUpItem)
		{
			AMannequinArmor* armor = Cast<AMannequinArmor>(OverlappingPlayer->PickedUpItem);

			if (armor)
			{
				if (OverlappingPlayer->IsInteracting)
				{
					armor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

					switch (armor->MannequinPart)
					{
					case EMannaquinPart::Helmet:
						EquippedArmor.HelmetEquipped = true;
						armor->SetActorLocation(HelmetPosition->GetComponentLocation());
						EquippedArray.Add(armor);
						break;
					case EMannaquinPart::Armor:
						EquippedArmor.ArmorEquipped = true;
						armor->SetActorLocation(ArmorPosition->GetComponentLocation());
						EquippedArray.Add(armor);
						break;
					case EMannaquinPart::Footwear:
						EquippedArmor.FootwearEquipped = true;
						armor->SetActorLocation(FootwearPosition->GetComponentLocation());
						EquippedArray.Add(armor);
						break;
					}

					if (EquipSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
					}

					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->PickedUpItem = nullptr;
					OverlappingPlayer->IsInteracting = false;
				}

			}
		}
	}

}

void AMannequin::CheckArmorEquipped_Implementation()
{
	RPCCheckArmorEquipped();
}

void AMannequin::RPCCheckArmorEquipped_Implementation()
{
	if (!EquippedArmor.ArmorEquipped)
	{
		MannequinEquiped = false;
		return;
	}

	if (!EquippedArmor.FootwearEquipped)
	{
		MannequinEquiped = false;
		return;
	}

	if (!EquippedArmor.HelmetEquipped)
	{
		MannequinEquiped = false;
		return;
	}

	MannequinEquiped = true;
}

void AMannequin::CheckCorrectArmor_Implementation()
{
	RPCCheckCorrectArmor();
}

void AMannequin::RPCCheckCorrectArmor_Implementation()
{
	for (int i = 0; i < EquippedArray.Num(); i++)
	{
		if (EquippedArray[i]->MannequinNumber != MannequinNumber)
		{
			CorrectArmor = false;
			return;
		}
	}

	CorrectArmor = true;
}


void AMannequin::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			if (playerActor)
			{
				OverlappingPlayer = playerActor;
				if (playerActor->PickedUpItem)
				{
					AMannequinArmor* armor = Cast<AMannequinArmor>(playerActor->PickedUpItem);

					if (armor)
					{
						switch (armor->MannequinPart)
						{
						case EMannaquinPart::Helmet:
							if (EquippedArmor.HelmetEquipped)
							{
								return;
							}
							break;
						case EMannaquinPart::Armor:
							if (EquippedArmor.ArmorEquipped)
							{
								return;
							}
							break;
						case EMannaquinPart::Footwear:
							if (EquippedArmor.FootwearEquipped)
							{
								return;
							}
							break;
						}
						OverlappingPlayer->CanInteract = true;
					}
				}

				CheckCorrectArmor();
				CheckArmorEquipped();

				if ((!CorrectArmor && MannequinEquiped))
				{
					OverlappingPlayer->CanInteract = true;
				}
			}
		}
	}
}

void AMannequin::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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