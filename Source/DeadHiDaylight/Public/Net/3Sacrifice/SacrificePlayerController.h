// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SacrificePlayerState.h"
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

	/*
	 *	Server Only
	 */
	UPROPERTY()
	TObjectPtr<class ASacrificeGameMode> GameMode = nullptr;
	UPROPERTY()
	TObjectPtr<class UDHDGameInstance> ServerGameInstance = nullptr;
	UPROPERTY()
	TObjectPtr<class ASacrificePlayerState> SacrificePlayerState = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USacrificeCommonHUD> HudFactory;
	UPROPERTY()
	TObjectPtr<class USacrificeCommonHUD> Hud = nullptr;
	
	bool bIsFreeMode = true;
	int SpectatorIndex = 0;
	
	void PrevPlayer();
	void NextPlayer();
	void ToggleCamera();
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_RequestCallbackWithGuid();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestCreatePawn(FGuid Guid);

	UFUNCTION(Client, Reliable)
	void ClientRPC_DisplayHUD();

	UFUNCTION(Client, Reliable)
	void ClientRPC_OnSkillCheck(const float Min, const float Max, const float GreatRange);
};