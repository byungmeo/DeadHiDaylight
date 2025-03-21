// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfireClientUI.h"

#include "CampfireGameMode.h"
#include "Components/TextBlock.h"
#include "DeadHiDaylight/DeadHiDaylight.h"

void UCampfireClientUI::RequestSelect(const bool bIsSlasher)
{
	NET_LOG(LogTemp, Warning, TEXT("ServerRPC_RequestSelect_Implementation : %hs"), bIsSlasher ? "Slasher" : "Camper");

	auto* PlayerController = Cast<ACampfirePlayerController>(GetOwningPlayer());
	PlayerController->ServerRPC_RequestSelect(bIsSlasher);
}

void UCampfireClientUI::UpdateSelectedSlot(const bool bIsSlasher)
{
	if (bIsSlasher)
	{
		TextCamperSlot->SetColorAndOpacity(FSlateColor(EStyleColor::White));
		TextSlasherSlot->SetColorAndOpacity(FSlateColor(EStyleColor::AccentGreen));
	}
	else
	{
		TextSlasherSlot->SetColorAndOpacity(FSlateColor(EStyleColor::White));
		TextCamperSlot->SetColorAndOpacity(FSlateColor(EStyleColor::AccentGreen));
	}
}

void UCampfireClientUI::UpdateSlot(const int SlasherCount, const int CamperCount)
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
