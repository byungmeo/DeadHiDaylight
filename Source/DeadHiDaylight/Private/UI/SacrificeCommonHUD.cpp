// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeCommonHUD.h"

#include "SacrificeGameState.h"

void USacrificeCommonHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* GameState = Cast<ASacrificeGameState>(GetWorld()->GetGameState()))
	{
		GameState->OnRepGeneratorCount.AddDynamic(this, &USacrificeCommonHUD::OnRepGeneratorCount);
		OnRepGeneratorCount(GameState->ReqGeneratorCount);
	}
}
