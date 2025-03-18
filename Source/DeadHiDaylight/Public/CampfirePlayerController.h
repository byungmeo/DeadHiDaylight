// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CampfirePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ACampfirePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void ClientRPC_AddWidget(TSubclassOf<UUserWidget> WidgetClass);
};
