// Fill out your copyright notice in the Description page of Project Settings.


#include "Pushable.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
APushable::APushable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->OnComponentHit.AddDynamic(this, &APushable::OnCompHit);
}

// Called when the game starts or when spawned
void APushable::BeginPlay()
{
	Super::BeginPlay();
	
	StartingTransform = GetActorTransform();


}

// Called every frame
void APushable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APushable::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if ((OtherActor) && (OtherActor != this))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if (player)
		{
			if (player->CanPush)
			{
				player->IsPushing = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
			}
		}
	}
}