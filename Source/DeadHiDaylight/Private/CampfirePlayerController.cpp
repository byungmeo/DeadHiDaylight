// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfirePlayerController.h"

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
