// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerFPSGameMode.h"
#include "MultiplayerFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerFPSGameMode::AMultiplayerFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
