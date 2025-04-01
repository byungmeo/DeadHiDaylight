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
	FText Nickname;
	
	/*
	 *	Server Only
	 */
	TMap<FGuid, EPlayerRole> RoleMap;
	
	/*
	 *	Server Only
	 */
	TMap<FGuid, FText> NameMap;
	
	/**
	 * 생존자의 닉네임과 생존자 생존 여부\n
	 * 0 : 생존 (게임이 완전히 끝나기 전에 탈출 또는 사망하여 해당 시점에 생존해있는 다른 생존자)\n
	 * 1 : 탈출\n
	 * 2 : 사망
	 */
	TMap<FString, int> MatchResultMap;
};