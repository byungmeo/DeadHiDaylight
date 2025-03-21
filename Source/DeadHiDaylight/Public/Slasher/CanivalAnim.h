// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CanivalAnim.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UCanivalAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class ACanival* Canival = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly)
	FRotator CamRotator = FRotator::ZeroRotator;
	
	UPROPERTY(BlueprintReadOnly)
	double Speed = 0.0;
	UPROPERTY(BlueprintReadOnly)
	int Direction;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayLeftClickAnimation();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayWipeAnimation();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayKickAnimation();
	
	UFUNCTION(BlueprintCallable)
	void CompletedKickAnimation();
};
