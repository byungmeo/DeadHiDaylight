// Fill out your copyright notice in the Description page of Project Settings.


#include "Slasher/CanivalUI.h"
#include "Components/ProgressBar.h"

void UCanivalUI::StartCharge()
{
	ChargeProgress = 0.0f;
	if (ProgressBar)
	{
		ProgressBar->SetPercent(ChargeProgress);
	}
    
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UCanivalUI::UpdateCS, 0.05f, true);
	}
}

void UCanivalUI::UpdateCS()
{
	ChargeProgress += 0.05f / TotalChargeTime;
	if (ProgressBar)
	{
		ProgressBar->SetPercent(ChargeProgress);
	}
    
	if (ChargeProgress >= 1.0f)
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
		}
		ChargeProgress = 1.0f;
		if (ProgressBar)
		{
			ProgressBar->SetPercent(ChargeProgress);
		}
	}
}