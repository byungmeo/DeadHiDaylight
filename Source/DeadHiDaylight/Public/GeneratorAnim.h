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
	bool bPowerOn = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	bool bIsExplosion = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float PowerGauge = 0.0f;

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnExplosion();
};
