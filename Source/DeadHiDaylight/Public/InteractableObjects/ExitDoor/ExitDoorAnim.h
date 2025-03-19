// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ExitDoorAnim.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UExitDoorAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class AExitDoor> ExitDoor = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDoorOpened = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsActivating = false;
	
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void OnOpenExitDoor();
};
