// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfireGameMode.h"

#include "CampfireGameState.h"
#include "CampfirePlayerController.h"
#include "DHDGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "DeadHiDaylight/DeadHiDaylight.h"

ACampfireGameMode::ACampfireGameMode()
{
	GameStateClass = ACampfireGameState::StaticClass();
	PlayerControllerClass = ACampfirePlayerController::StaticClass();
}

void ACampfireGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACampfireGameMode::InitGameState()
{
	Super::InitGameState();
	CampfireGameState = Cast<ACampfireGameState>(GameState);
}

void ACampfireGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NET_LOG(LogTemp, Warning, TEXT("ACampfireGameMode::PostLogin"));

	auto* Controller = Cast<ACampfirePlayerController>(NewPlayer);
	const FGuid NewGuid = FGuid::NewGuid();
	Controller->Guid = NewGuid;
	Controller->ClientRPC_SetGuid(NewGuid);
	
	if (Controller->IsLocalPlayerController())
	{
		CampfireGameState->RoleMap.Add(NewGuid, EPlayerRole::EPR_Observer);
		Controller->SelectedRole = EPlayerRole::EPR_Observer;
		auto* Widget = CreateWidget(NewPlayer, ObserverWidgetFactory);
		Widget->AddToViewport();
	}
	else
	{
		CampfireGameState->RoleMap.Add(NewGuid, EPlayerRole::EPR_None);
		Controller->ClientRPC_AddWidget(ClientWidgetFactory);
	}
}

void ACampfireGameMode::StartSacrifice()
{
	if (CampfireGameState->SlasherCount <= 0)
	{
		SCREEN_LOG(TEXT("SlasherCount is 0"));
		return;
	}
	if (CampfireGameState->CamperCount <= 0)
	{
		SCREEN_LOG(TEXT("CamperCount is 0"));
		return;
	}
	ServerGameInstance->RoleMap = CampfireGameState->RoleMap;
	GetWorld()->ServerTravel("/Game/Common/Maps/SacrificeMap");
}