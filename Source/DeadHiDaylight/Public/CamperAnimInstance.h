// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CamperAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UCamperAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	float moveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bWalk = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bRun = false;

	void IsWalk();
	void IsRun();
};
