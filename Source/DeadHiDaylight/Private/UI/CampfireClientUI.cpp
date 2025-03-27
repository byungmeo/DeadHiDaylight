// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfireClientUI.h"

#include "CampfirePlayerController.h"
#include "DHDGameInstance.h"
#include "Components/TextBlock.h"
#include "DeadHiDaylight/DeadHiDaylight.h"

void UCampfireClientUI::RequestSelect(const bool bIsSlasher) const
{
	NET_LOG(LogTemp, Warning, TEXT("UCampfireClientUI::RequestSelect : %hs"), bIsSlasher ? "Slasher" : "Camper");

	auto* PlayerController = Cast<ACampfirePlayerController>(GetOwningPlayer());
	const EPlayerRole ReqRole = bIsSlasher ? EPlayerRole::EPR_Slasher : EPlayerRole::EPR_Camper;
	if (PlayerController->SelectedRole != ReqRole)
	{
		PlayerController->ServerRPC_RequestSelect(ReqRole);
	}
}

void UCampfireClientUI::UpdateSelectedSlot(const EPlayerRole SelectedRole) const
{
	if (SelectedRole == EPlayerRole::EPR_Slasher)
	{
		TextSlasherSlot->SetColorAndOpacity(FSlateColor(EStyleColor::AccentGreen));
		TextCamperSlot->SetColorAndOpacity(FSlateColor(EStyleColor::White));
	}
	else if (SelectedRole == EPlayerRole::EPR_Camper)
	{
		TextCamperSlot->SetColorAndOpacity(FSlateColor(EStyleColor::AccentGreen));
		TextSlasherSlot->SetColorAndOpacity(FSlateColor(EStyleColor::White));
	}
	else
	{
		//
	}
}

void UCampfireClientUI::UpdateSlot(const int SlasherCount, const int CamperCount) const
{
	if (TextSlasherSlot)
	{
		TextSlasherSlot->SetText(FText::FromString(FString::Printf(TEXT("Slasher %d / 1"), SlasherCount)));
	}

	if (TextCamperSlot)
	{
		TextCamperSlot->SetText(FText::FromString(FString::Printf(TEXT("Camper %d / 4"), CamperCount)));
	}
}
