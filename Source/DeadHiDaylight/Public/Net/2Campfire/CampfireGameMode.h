// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DHDGameModeBase.h"
#include "CampfireGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ACampfireGameMode : public ADHDGameModeBase
{
	GENERATED_BODY()

public:
	ACampfireGameMode();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION(BlueprintCallable)
	void GameStart();

	int Temp = 0;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ObserverWidgetFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ClientWidgetFactory;
};
