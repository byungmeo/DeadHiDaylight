// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MeatHookAnim.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UMeatHookAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class AMeatHook> MeatHook = nullptr;

	virtual void NativeBeginPlay() override;
};
