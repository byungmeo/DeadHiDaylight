// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeCommonHUD.h"

#include "SacrificeGameState.h"
#include "SacrificePlayerState.h"
#include "DeadHiDaylight/DeadHiDaylight.h"

void USacrificeCommonHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* GameState = Cast<ASacrificeGameState>(GetWorld()->GetGameState()))
	{
		OnRepGeneratorCount(GameState->ReqGeneratorCount);
		GameState->OnRepGeneratorCount.AddDynamic(this, &USacrificeCommonHUD::OnRepGeneratorCount);
	}
}

void USacrificeCommonHUD::AddCamperState(class ASacrificePlayerState* CamperState)
{
	NET_LOG(LogTemp, Warning, TEXT("USacrificeCommonHUD::AddCamperState Role=%d"), CamperState->UserState.PlayerRole);
	OnUpdatedCamperState(CamperState->UserState);
	CamperState->OnUpdatedUserState.AddDynamic(this, &USacrificeCommonHUD::OnUpdatedCamperState);
}