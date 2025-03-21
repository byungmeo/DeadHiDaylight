// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CampfireGameState.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ACampfireGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdateSlot(const int SlasherCount, const int CamperCount);
};
