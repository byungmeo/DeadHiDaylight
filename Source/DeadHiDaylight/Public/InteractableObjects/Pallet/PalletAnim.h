// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pallet.h"
#include "Animation/AnimInstance.h"
#include "PalletAnim.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UPalletAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeBeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	APallet* Pallet;

	UFUNCTION(BlueprintImplementableEvent)
	void OnFallGround();
};
