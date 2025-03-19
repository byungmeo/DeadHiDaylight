// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DHDGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ADHDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADHDGameModeBase();

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TObjectPtr<class UDHDGameInstance> ServerGameInstance = nullptr;
};