// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SacrificePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ASacrificePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
	UPROPERTY()
	class ASacrificeGameMode* GameMode = nullptr;
	UPROPERTY()
	TArray<APawn*> Players;
	
	bool bIsFreeMode = true;
	int SpectatorIndex = 0;
	void PrevPlayer();
	void NextPlayer();
	void ToggleCamera();
};
