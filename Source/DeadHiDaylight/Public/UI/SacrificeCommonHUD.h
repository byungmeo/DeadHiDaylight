// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SacrificePlayerState.h"
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
	virtual void NativeConstruct() override;
	
	/*UFUNCTION(BlueprintImplementableEvent)
	void UpdateCamper(const TArray<struct FCamperState>& StateList);*/
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSkillCheck(float Min, float Max, float GreatRange);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRepGeneratorCount(int Count);

	void AddCamperState(class ASacrificePlayerState* CamperState);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdatedCamperState(const FUserState& UserState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnVisibleGaugeBar(const FText& TitleText, const float InitFillAmount);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateGaugeBar(const float FillAmount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHiddenGaugeBar();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdatedNearPoint(class UInteractionPoint* Point, const FText& Description);

	// 피를 일단 표시했다가 Delay걸어서 2초 뒤에 사라지게 한다
	UFUNCTION(BlueprintImplementableEvent)
	void OnDisplayBlood();
};
