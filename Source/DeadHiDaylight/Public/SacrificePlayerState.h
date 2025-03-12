// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SacrificePlayerState.generated.h"

USTRUCT()
struct FCamperStats
{
	GENERATED_BODY()

	// TODO: 게임 전체에 공유되어야 하는 갈고리 걸린 횟수, 부상 or 지렁이 상태 등등 정의
};

USTRUCT()
struct FSlasherStats
{
	GENERATED_BODY()

	// 게임 전체에 공유되어야 하는 요소가 딱히 없음
	// 필요하면 추후 추가
};

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ASacrificePlayerState : public APlayerState
{
	GENERATED_BODY()
};
