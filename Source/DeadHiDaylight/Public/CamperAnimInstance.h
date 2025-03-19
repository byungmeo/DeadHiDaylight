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
	float moveSpeed; // 현재 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bWalk = false; // 일반 걷기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bRun = false; // 일반 뛰기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bCrouch = false; // 앉기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bStartRepair = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bEndRepair = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bInjure = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	int32 randValue = 0; // 앉아서 걷는 애니메이션 선택 하는 값
	
	void IsWalk(); // 걷고 있는지 체크
	void IsRun(); // 뛰고 있는지 체크
	void IsCrouch(bool value); // 앉아 있는지 체크

	int32 RandomValue(); // 1, 2중 값 반환

	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayRepairAnimation(FName sectionName);

	UFUNCTION()
	void AnimNotify_StartGen();
	UFUNCTION()
	void AnimNotify_GenEnd();
};
