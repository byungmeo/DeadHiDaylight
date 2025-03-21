// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfirePlayerController.h"

#include "CampfireClientUI.h"
#include "CampfireGameMode.h"
#include "DHDGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Kismet/GameplayStatics.h"

void ACampfirePlayerController::ClientRPC_AddWidget_Implementation(TSubclassOf<UUserWidget> WidgetClass)
{
	if (IsLocalPlayerController() && WidgetClass)
	{
		CampfireWidget = CreateWidget<UUserWidget>(this, WidgetClass);
		if (CampfireWidget)
		{
			CampfireWidget->AddToViewport();
		}
	}
}

void ACampfirePlayerController::ClientRPC_SetGuid_Implementation(const FGuid NewGuid)
{
	auto* ClientGameInstance = Cast<UDHDGameInstance>(GetGameInstance());
	if (ClientGameInstance)
	{
		NET_LOG(LogTemp, Warning, TEXT("ClientRPC_SetGuid_Implementation %s"), *NewGuid.ToString());
		ClientGameInstance->Guid = NewGuid;
	}
}

void ACampfirePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
}

void ACampfirePlayerController::UpdateSlot(const int SlasherCount, const int CamperCount)
{
	if (HasAuthority())
	{
		// UCampfireServerUI* ServerUI = Cast<UCampfireServerUI>(CampfireWidget);
		// ServerUI->UpdateSlot(SlasherCount, CamperCount);
	}
	else
	{
		UCampfireClientUI* ClientUI = Cast<UCampfireClientUI>(CampfireWidget);
		ClientUI->UpdateSlot(SlasherCount, CamperCount);
	}
	
}

void ACampfirePlayerController::ClientRPC_UpdateSelectedSlot_Implementation(const bool bIsSlasher)
{
	UCampfireClientUI* ClientUI = Cast<UCampfireClientUI>(CampfireWidget);
	ClientUI->UpdateSelectedSlot(bIsSlasher);
}

void ACampfirePlayerController::ServerRPC_RequestSelect_Implementation(const bool bIsSlasher)
{
	auto* GameMode = Cast<ACampfireGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->RequestSelect(this, bIsSlasher);
}
