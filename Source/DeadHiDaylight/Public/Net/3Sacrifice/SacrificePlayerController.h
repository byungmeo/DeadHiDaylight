// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SacrificePlayerController.generated.h"

enum class ESkillCheckResult : uint8;
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
	TObjectPtr<class UDHDGameInstance> LocalGameInstance = nullptr;
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
	
	void RequestCallbackWithGuid();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestCreatePawn(FGuid Guid);

	UPROPERTY()
	TObjectPtr<AActor> SkillCheckableObject = nullptr;
	UFUNCTION(Client, Reliable)
	void ClientRPC_OnSkillCheck(AActor* Obj, const float Min, const float Max, const float GreatRange);
	UFUNCTION(BlueprintCallable)
	void SkillCheckFinish(const ESkillCheckResult Result);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SkillCheckFinish(AActor* Obj, const ESkillCheckResult Result);

	void DisplayHud();
	void OnCreatedNewCamperState(class ASacrificePlayerState* CamperState);
};