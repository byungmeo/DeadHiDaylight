// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfireGameMode.h"

#include "CampfirePlayerController.h"
#include "DHDGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Kismet/GameplayStatics.h"

ACampfireGameMode::ACampfireGameMode()
{
	PlayerControllerClass = ACampfirePlayerController::StaticClass();
}

void ACampfireGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACampfireGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NET_LOG(LogTemp, Warning, TEXT("ACampfireGameMode::PostLogin"));

	auto* Controller = Cast<ACampfirePlayerController>(NewPlayer);
	const FGuid NewGuid = FGuid::NewGuid();
	Controller->ClientRPC_SetGuid(NewGuid);
	
	if (Controller->IsLocalPlayerController())
	{
		auto* Widget = CreateWidget(NewPlayer, ObserverWidgetFactory);
		Widget->AddToViewport();
		ServerGameInstance->RoleMap.Add(NewGuid, EPlayerRole::EPR_Observer);
	}
	else
	{
		Controller->ClientRPC_AddWidget(ClientWidgetFactory);
		if (Temp <= 0)
		{
			ServerGameInstance->RoleMap.Add(NewGuid, EPlayerRole::EPR_Camper);
		}
		else
		{
			ServerGameInstance->RoleMap.Add(NewGuid, EPlayerRole::EPR_Slasher);
		}

		Temp++;
	}
}

void ACampfireGameMode::GameStart()
{
	GetWorld()->ServerTravel("/Game/Common/Maps/SacrificeMap");
}
