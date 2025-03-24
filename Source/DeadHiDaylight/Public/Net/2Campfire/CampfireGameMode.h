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
	virtual void InitGameState() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
	
public:
	UPROPERTY()
	TObjectPtr<class ACampfireGameState> CampfireGameState = nullptr;
	
	int Temp = 0;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ObserverWidgetFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ClientWidgetFactory;
	
	UFUNCTION(BlueprintCallable)
    void StartSacrifice();
};
