// Fill out your copyright notice in the Description page of Project Settings.


#include "Mannequin.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

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

	//If there is a overlapping player
	if (OverlappingPlayer)
	{
		//Checks whether all the armor is equipped
		CheckArmorEquipped();

		//If the mannequin has all the armor pieces equipped
		if (MannequinEquiped)
		{
			//Checks if the armor is correct or not
			CheckCorrectArmor();

			//If the armor is incorrect
			if (!CorrectArmor)
			{
				//If the player interacts
				if (OverlappingPlayer->IsInteracting)
				{
					//Iterate through all the equipped armor pieces
					for (int i = 0; i < EquippedArray.Num(); i++)
					{
						//If the index is valid
						if (EquippedArray[i])
						{
							//If the equipper armor array index's mannequin number is incorrect
							if(EquippedArray[i]->MannequinNumber != MannequinNumber)
							{
								//Launches the armor piece away from the mannequin and resets the variables to allow it to be picked up again
								EquippedArray[i]->Mesh->SetCollisionProfileName("IgnoreOnlyPawn");
								EquippedArray[i]->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
								EquippedArray[i]->Launch(FVector(3, 3, 3));
								EquippedArray[i]->Player = nullptr;
								EquippedArray[i]->AttachedToMannequin = true;

								//Set all the equipped bools to false for the corresponding part 
								switch (EquippedArray[i]->MannequinPart)
								{
								case EMannaquinPart::Helmet:
									EquippedArmor.HelmetEquipped = false;
									break;
								case EMannaquinPart::Armor:
									EquippedArmor.ArmorEquipped = false;
									break;
								case EMannaquinPart::Footwear:
									EquippedArmor.FootwearEquipped = false;
									break;
								}

								//Remove this armor from the array
								EquippedArray.RemoveAt(i);
							}
						}
	
					}

					//Play the remove sound
					if (RemoveSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), RemoveSound, GetActorLocation());
					}

				}
			}

		}

		//If the player has an item
		if (OverlappingPlayer->PickedUpItem)
		{
			AMannequinArmor* armor = Cast<AMannequinArmor>(OverlappingPlayer->PickedUpItem);

			//If the item is a mannequin armor piece
			if (armor)
			{
				//If the player presses interact
				if (OverlappingPlayer->IsInteracting)
				{
					//Make the armor attached to the mannequin
					armor->AttachedToMannequin = true;
					armor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

					//Check for where the armor piece should be placed
					//Then updated the equipped armor struct
					//Snap the armor piece to set location
					//And add it to the equipped armor array
					switch (armor->MannequinPart)
					{
					case EMannaquinPart::Helmet:
						EquippedArmor.HelmetEquipped = true;
						armor->SetActorLocation(HelmetPosition->GetComponentLocation());
						armor->SetActorRotation(HelmetPosition->GetComponentRotation());
						EquippedArray.Add(armor);
						break;
					case EMannaquinPart::Armor:
						EquippedArmor.ArmorEquipped = true;
						armor->SetActorLocation(ArmorPosition->GetComponentLocation());
						armor->SetActorRotation(ArmorPosition->GetComponentRotation());
						EquippedArray.Add(armor);
						break;
					case EMannaquinPart::Footwear:
						EquippedArmor.FootwearEquipped = true;
						armor->SetActorLocation(FootwearPosition->GetComponentLocation());
						armor->SetActorRotation(FootwearPosition->GetComponentRotation());
						EquippedArray.Add(armor);
						break;
					}

					//Play the equip sound
					if (EquipSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
					}

					//Stop the player from further interacting
					OverlappingPlayer->CanInteract = false;
					OverlappingPlayer->IsInteracting = false;
					//Remove the item from the players hand
					OverlappingPlayer->PickedUpItem = nullptr;
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
	//Checks whether the certain armor piece is equipped or not

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

	//If all armor pieces are equipped, set mannequin equipped to true
	MannequinEquiped = true;
}

void AMannequin::CheckCorrectArmor_Implementation()
{
	RPCCheckCorrectArmor();
}

void AMannequin::RPCCheckCorrectArmor_Implementation()
{
	//Iterate through the armor
	for (int i = 0; i < EquippedArray.Num(); i++)
	{
		//Check if the armor's number matches to the mannequin's number
		if (EquippedArray[i]->MannequinNumber != MannequinNumber)
		{
			//If they aren't the same set correct armor to false and stop iterating
			CorrectArmor = false;
			return;
		}
	}

	//If all armor pieces match, then set correct armor to true
	CorrectArmor = true;
}


void AMannequin::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OverlappingPlayer == nullptr)
		{
			APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);

			//If the overlapping actor is a player
			if (playerActor)
			{
				OverlappingPlayer = playerActor;

				//Check if the overlapping player has a item
				if (playerActor->PickedUpItem)
				{
					AMannequinArmor* armor = Cast<AMannequinArmor>(playerActor->PickedUpItem);

					//If the item is a mannequin armor piece 
					if (armor)
					{
						//Check if that armor piece is already equipped or not
						//If it is don't allow the player to interact
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

						//If the armor isn't already equipped, allow the player to interact
						OverlappingPlayer->CanInteract = true;

						//Change the players interact type to insert
						OverlappingPlayer->InteractType = EInteractType::Insert;
					}
				}

				//Check if all the armor has been equipped and the armor is incorrect
				CheckCorrectArmor();
				CheckArmorEquipped();

				//If they are, then also allow the player to interact (to remove the armor)
				if ((!CorrectArmor && MannequinEquiped))
				{
					OverlappingPlayer->CanInteract = true;
					//Change the players interact type to insert
					OverlappingPlayer->InteractType = EInteractType::Insert;
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