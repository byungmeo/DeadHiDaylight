// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SacrificeGameState.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ASacrificeGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPowerOn();
	
public:
	TArray<class AGenerator*> Generators;

	// 현재 탈출구 활성화에 필요한 발전기의 수
	UPROPERTY(BlueprintReadOnly)
	int ReqGeneratorCount = 5;
};
