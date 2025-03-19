// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SacrificePlayerController.generated.h"

enum class EPlayerRole : uint8;
/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ASacrificePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
	UPROPERTY()
	class ASacrificeGameMode* GameMode = nullptr;
	
	bool bIsFreeMode = true;
	int SpectatorIndex = 0;
	void PrevPlayer();
	void NextPlayer();
	void ToggleCamera();
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_RequestCallbackWithGuid();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestCreatePawn(FGuid Guid);
};