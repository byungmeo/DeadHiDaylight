// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DHDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UDHDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateRoom();
	UFUNCTION(BlueprintCallable)
	void JoinRoom(const FString& ServerAddress);
};
