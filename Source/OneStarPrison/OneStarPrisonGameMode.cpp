// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneStarPrisonGameMode.h"
#include "OneStarPrisonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOneStarPrisonGameMode::AOneStarPrisonGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}
