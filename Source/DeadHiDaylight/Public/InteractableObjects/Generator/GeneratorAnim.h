// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GeneratorAnim.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UGeneratorAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class AGenerator> Generator = nullptr;
	UPROPERTY(BlueprintReadOnly)
	bool bPowerOn = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsExplosion = false;
	UPROPERTY(BlueprintReadOnly)
	float PowerGauge = 0.0f;
	
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPowerOn();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnExplosion();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBreak();
};
