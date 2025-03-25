// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfirePlayerController.h"

#include "CampfireClientUI.h"
#include "CampfireGameState.h"
#include "DHDGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Net/UnrealNetwork.h"

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
		Guid = NewGuid;
	}
}

void ACampfirePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
}

void ACampfirePlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACampfirePlayerController, SelectedRole);
}

void ACampfirePlayerController::OnRep_SelectedRole() const
{
	if (HasAuthority() || SelectedRole == EPlayerRole::EPR_None)
	{
		return;
	}
	
	UCampfireClientUI* ClientUI = Cast<UCampfireClientUI>(CampfireWidget);
	ClientUI->UpdateSelectedSlot(SelectedRole);
}

void ACampfirePlayerController::ServerRPC_RequestSelect_Implementation(const EPlayerRole ReqRole)
{
	auto* GameState = Cast<ACampfireGameState>(GetWorld()->GetGameState());
	GameState->ServerRPC_RequestSelect(this, ReqRole);
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

void ACampfirePlayerController::UpdateSelectedSlot(const bool bIsSlasher)
{
	
}