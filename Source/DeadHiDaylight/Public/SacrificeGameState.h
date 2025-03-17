// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SacrificeGameState.generated.h"

/**
 * 잠식 단계
 */
UENUM()
enum class EStrugglePhase : uint8
{
	ESP_First,
	ESP_Second,
	ESP_Third
};

/**
 * 생존자 건강 상태
 */
UENUM()
enum class ECamperHealth : uint8
{
	ECH_Healthy,
	ECH_Injury,
	ECH_Endurance,
	ECH_Moribund,
	ECH_Dead
};

/**
 * 생존자의 상태 중 게임 전체에 공유되어야 하는 상태
 */
USTRUCT()
struct FCamperStats
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName Name = FName(NAME_None);
	
	UPROPERTY()
	EStrugglePhase StrugglePhase = EStrugglePhase::ESP_First;

	UPROPERTY()
	ECamperHealth Health = ECamperHealth::ECH_Healthy;
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
	// TODO: GameMode 등을 통해서 전달받도록 변경하기
	TArray<class AGenerator*> Generators;
	UPROPERTY(BlueprintReadOnly)
	int ReqGeneratorCount = 5;

	TArray<FCamperStats> CamperStats;
};
