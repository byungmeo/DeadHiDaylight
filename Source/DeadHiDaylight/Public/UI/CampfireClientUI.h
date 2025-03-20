// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CampfireClientUI.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UCampfireClientUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TextSlasherSlot = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TextCamperSlot = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void RequestSelect(const bool bIsSlasher);
	
	void UpdateSelectedSlot(const bool bIsSlasher);
	
	void UpdateSlot(const int SlasherCount, const int CamperCount);
};