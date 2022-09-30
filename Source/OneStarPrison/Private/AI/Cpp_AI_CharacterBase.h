#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cpp_AI_CharacterBase.generated.h"

UCLASS()
class ONESTARPRISON_API ACpp_AI_CharacterBase : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACpp_AI_CharacterBase();

	// Called every frame
	virtual void Tick(float const DeltaTime) override;

	virtual void AttackStart();
	virtual void AttackEnd();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RightFistCollisionBox;
};