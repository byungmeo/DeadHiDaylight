// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DHDGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "SacrificePlayerState.generated.h"

enum class EPlayerRole : uint8;

/**
 * 잠식 단계
 */
UENUM(BlueprintType)
enum class EStrugglePhase : uint8
{
	ESP_First,
	ESP_Second,
	ESP_Third
};

/**
 * 생존자 건강 상태
 */
UENUM(BlueprintType)
enum class ECamperHealth : uint8
{
	ECH_Healthy,
	ECH_Injury,
	ECH_Endurance,
	ECH_Moribund,
	ECH_Dead
};

USTRUCT(BlueprintType)
struct FBaseState
{
	GENERATED_BODY()
	FName Name = FName(NAME_None);
	EPlayerRole PlayerRole = EPlayerRole::EPR_None;
};

USTRUCT(BlueprintType)
struct FObserverState : public FBaseState
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FCamperState : public FBaseState
{
	GENERATED_BODY()

	UPROPERTY()
	EStrugglePhase StrugglePhase = EStrugglePhase::ESP_First;

	UPROPERTY()
	ECamperHealth Health = ECamperHealth::ECH_Healthy;
};

USTRUCT(BlueprintType)
struct FSlasherState : public FBaseState
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ASacrificePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FBaseState PlayerState;
};
