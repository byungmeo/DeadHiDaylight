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
	TObjectPtr<class AGenerator> Generator = nullptr;
	UPROPERTY(BlueprintReadOnly)
	float PowerGauge = 0.0f;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
