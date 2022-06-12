// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickupable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlayerCharacter.h"

//#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

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
}

// Called every frame
void APickupable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupable::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	if ((OtherActor) && (OtherActor != this))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (!player)
		{
			Mesh->SetSimulatePhysics(true);
			ProjectileMovement->Deactivate();
		}
	}
}

void APickupable::Launch(FVector _Velocity)
{
	ProjectileMovement->Velocity = _Velocity;
	ProjectileMovement->Activate();
}