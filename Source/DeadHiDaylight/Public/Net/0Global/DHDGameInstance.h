// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DHDGameInstance.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	EPR_None		UMETA(DisplayName = "None"),
	EPR_Observer	UMETA(DisplayName = "Observer"),
	EPR_Slasher		UMETA(DisplayName = "Slasher"),
	EPR_Camper		UMETA(DisplayName = "Camper")
};

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UDHDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FGuid Guid;
	TMap<FGuid, EPlayerRole> RoleMap;
};
