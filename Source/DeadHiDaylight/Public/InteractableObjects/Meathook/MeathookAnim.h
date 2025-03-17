// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MeathookAnim.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UMeathookAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class AMeathook> Meathook = nullptr;

	virtual void NativeBeginPlay() override;
};
