// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CanivalUI.generated.h"

/**
 * 
 */
UCLASS()
class UCanivalUI : public UUserWidget
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable, category = "ChainSaw")
	void StartCharge();
	UFUNCTION(BlueprintCallable, category = "ChainSaw")
	void UpdateCS();

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ProgressBar;

	float ChargeProgress;

	FTimerHandle ChargeTimerHandle;

	float TotalChargeTime = 3.0f;
};
