// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SacrificeGameState.generated.h"

/**
 * 생존자의 상태 중 게임 전체에 공유되어야 하는 상태
 */
USTRUCT()
struct FCamperStats
{
	GENERATED_BODY()

	// TODO: 갈고리 걸린 횟수, 체력, 집착 등
};

/**
 * 살인마의 상태 중 게임 전체에 공유되어야 하는 상태
 */
USTRUCT()
struct FSlasherStats
{
	GENERATED_BODY()
};

/**
 * 희생제에서 생존자, 살인마 모두가 가지고 있는(혹은 알아야 하는) 상태
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
