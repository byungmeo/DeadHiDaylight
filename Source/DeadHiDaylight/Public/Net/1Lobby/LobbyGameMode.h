// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();
	
public:
	UFUNCTION(BlueprintCallable)
	void CreateRoom();
	UFUNCTION(BlueprintCallable)
	void JoinRoom(const FString& ServerAddress);
};
