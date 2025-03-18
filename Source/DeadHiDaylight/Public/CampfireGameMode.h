// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CampfireGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ACampfireGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACampfireGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ObserverWidgetFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ClientWidgetFactory;
};
