// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfirePlayerController.h"

#include "DHDGameInstance.h"
#include "Blueprint/UserWidget.h"

void ACampfirePlayerController::ClientRPC_AddWidget_Implementation(TSubclassOf<UUserWidget> WidgetClass)
{
	if (IsLocalPlayerController() && WidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(this, WidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}

void ACampfirePlayerController::ClientRPC_SetGuid_Implementation(const FGuid NewGuid)
{
	auto* ClientGameInstance = Cast<UDHDGameInstance>(GetGameInstance());
	if (ClientGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClientRPC_SetGuid_Implementation %s"), *NewGuid.ToString());
		ClientGameInstance->Guid = NewGuid;
	}
}
