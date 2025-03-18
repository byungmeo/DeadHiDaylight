// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfireGameMode.h"

#include "CampfirePlayerController.h"
#include "Blueprint/UserWidget.h"

ACampfireGameMode::ACampfireGameMode()
{
	PlayerControllerClass = ACampfirePlayerController::StaticClass();
}

void ACampfireGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("ACampfireGameMode::PostLogin"));

	if (NewPlayer->IsLocalPlayerController())
	{
		auto* Widget = CreateWidget(NewPlayer, ObserverWidgetFactory);
		Widget->AddToViewport();
	}
	else
	{
		auto* Controller = Cast<ACampfirePlayerController>(NewPlayer);
		Controller->ClientRPC_AddWidget(ClientWidgetFactory);
	}
}
