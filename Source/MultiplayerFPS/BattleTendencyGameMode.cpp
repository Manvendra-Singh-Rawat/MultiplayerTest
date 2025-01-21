// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTendencyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ABattleTendencyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Don't use bUseSeamlessTravel on the map where you are tryingto connect to a server
	// as seamless travel means it won't disconnect from the current server so you will remain on the same IP
	bUseSeamlessTravel = true;

	PlayerCount = GameState.Get()->PlayerArray.Num();
	if (PlayerCount >= 10)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			// After sufficient players have joined we will make everyone travel to new level
			World->ServerTravel(FString("/Game/MyStuff/Maps/TestPlayableMap?listen"));
		}
	}
}

void ABattleTendencyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
