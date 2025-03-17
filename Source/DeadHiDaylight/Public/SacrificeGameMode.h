// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SacrificeGameMode.generated.h"

/**
 * 희생제에서 Listen Server(Observer)에서만 동작해야 하는, 알아야 하는
 */
UCLASS()
class DEADHIDAYLIGHT_API ASacrificeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASacrificeGameMode();
};
