// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickupable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlayerCharacter.h"

#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
APickupable::APickupable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	Mesh->OnComponentHit.AddDynamic(this, &APickupable::OnCompHit);
	RootComponent = Mesh;
	Player = nullptr;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Mesh);
	ProjectileMovement->SetAutoActivate(false);
	ProjectileMovement->InitialSpeed = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
}

// Called when the game starts or when spawned
void APickupable::BeginPlay()
{
	Super::BeginPlay();

	//Set its cache values to its value when the game starts
	cacheTransform = GetActorTransform();
	ParentActor = GetParentActor();
}

void APickupable::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupable, Player);
	DOREPLIFETIME(APickupable, IsInAir);

	DOREPLIFETIME(APickupable, Mass);

}

// Called every frame
void APickupable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If it is to be reset
	if (OnDisplay)
	{
		//If the a player has picked it up
		if (Player)
		{
			TakenFromDisplay = true;
			//Reset timer every time the player picks it up
			Timer = 0.0f;
		}
		else
		{
			//If the item has been picked up but no longer attached to a player
			if (TakenFromDisplay)
			{
				//When it hits something
				if (!IsInAir)
				{
					//Increment the timer over time
					Timer += DeltaTime;
				}
			}
		}

		//If the timer has reached the time before it respawns
		if (Timer > TimeBeforeItRespawns)
		{
			//Reset the bool
			TakenFromDisplay = false;

			//If a parent actor exists
			if (ParentActor)
			{
				//Attach to its parent actor
				AttachToActor(ParentActor, FAttachmentTransformRules::SnapToTargetIncludingScale);
			}

			//Resets its transform to its starting transform
			SetActorTransform(cacheTransform);

			//Disable physics
			Mesh->SetSimulatePhysics(false);

			//Play the return sound
			if (ReturnToDisplaySound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReturnToDisplaySound, GetActorLocation());
			}

			//Reset the timer
			Timer = 0.0f;
		}

	}

}

void APickupable::PlayPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	}
}

void APickupable::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor) && (OtherActor != this))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		//If the mesh hits a player that isn't the player
		if (!player)
		{
			//Set its physics to true and disables its projectile movement
			IsInAir = false;
			Mesh->SetSimulatePhysics(true);
			ProjectileMovement->Deactivate();
			Mesh->SetCollisionProfileName("BlockAll");
		}
	}
}

void APickupable::Launch(FVector _Velocity)
{
	//Activates its projectile movement and sets it velocity to follow a path
	IsInAir = true;
	ProjectileMovement->Velocity = _Velocity;
	ProjectileMovement->Activate();
}