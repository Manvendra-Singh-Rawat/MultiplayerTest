// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BattleTendencyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ABattleTendencyGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	int PlayerCount{ 0 };

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
