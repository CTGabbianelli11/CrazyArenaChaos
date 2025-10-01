// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrazyArenaChaosGameMode.h"
#include "CrazyArenaChaosCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACrazyArenaChaosGameMode::ACrazyArenaChaosGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
