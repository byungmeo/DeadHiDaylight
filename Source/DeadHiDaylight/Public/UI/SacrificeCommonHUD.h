// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SacrificeCommonHUD.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API USacrificeCommonHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnSkillCheck(float Min, float Max, float GreatRange);
};
