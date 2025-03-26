// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeCommonHUD.h"

#include "SacrificeGameState.h"
#include "SacrificePlayerState.h"

void USacrificeCommonHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto* GameState = Cast<ASacrificeGameState>(GetWorld()->GetGameState()))
	{
		GameState->OnRepGeneratorCount.AddDynamic(this, &USacrificeCommonHUD::OnRepGeneratorCount);
		OnRepGeneratorCount(GameState->ReqGeneratorCount);

		TArray<FUserState> CamperStates;
		for (auto PS : GameState->PlayerArray)
		{
			if (auto* SacrificePlayerState = Cast<ASacrificePlayerState>(PS))
			{
				if (SacrificePlayerState->UserState.PlayerRole == EPlayerRole::EPR_Camper)
				{
					AddCamper(SacrificePlayerState->UserState.Name);
					SacrificePlayerState->OnUpdatedUserState.AddDynamic(this, &USacrificeCommonHUD::OnUpdatedUserState);
				}
			}
		}
	}
}