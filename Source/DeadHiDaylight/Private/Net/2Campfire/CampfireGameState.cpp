// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfireGameState.h"

#include "CampfireClientUI.h"
#include "CampfirePlayerController.h"

void ACampfireGameState::MulticastRPC_UpdateSlot_Implementation(const int SlasherCount, const int CamperCount)
{
	if (HasAuthority())
	{
		return;
	}
	
	ACampfirePlayerController* PlayerController = Cast<ACampfirePlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		UCampfireClientUI* ClientUI = Cast<UCampfireClientUI>(PlayerController->CampfireWidget);
		if (ClientUI)
		{
			ClientUI->UpdateSlot(SlasherCount, CamperCount);
		}
	}
}
