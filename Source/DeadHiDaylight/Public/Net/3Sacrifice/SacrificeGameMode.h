// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DHDGameModeBase.h"
#include "SacrificePlayerController.h"
#include "SacrificeGameMode.generated.h"

/**
 * 희생제에서 Listen Server(Observer)에서만 동작해야 하는, 알아야 하는
 */
UCLASS()
class DEADHIDAYLIGHT_API ASacrificeGameMode : public ADHDGameModeBase
{
	GENERATED_BODY()

public:
	ASacrificeGameMode();

protected:
	virtual void InitGameState() override;
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

public:
	UPROPERTY()
	TObjectPtr<class ASacrificeGameState> SacrificeGameState = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACamper> CamperFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACanival> SlasherFactory;
	
	TArray<class ACamper*> Campers;
	TObjectPtr<class ACanival> Slasher = nullptr;
	TArray<class AGenerator*> Generators;
	TArray<class AMeatHook*> MeatHooks;

	void RequestCreatePawn(ASacrificePlayerController* Controller, EPlayerRole PlayerRole);
};
