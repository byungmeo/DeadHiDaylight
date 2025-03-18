// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DHDPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	Observer	UMETA(DisplayName = "Observer"),
	Slasher		UMETA(DisplayName = "Slasher"),
	Camper		UMETA(DisplayName = "Camper"),
	None		UMETA(DisplayName = "None")
};

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ADHDPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	EPlayerRole PlayerRole;
};
