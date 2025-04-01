// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CamperAnimInstance.generated.h"

enum class ECamperInteraction : uint8;
enum class ECamperHealth : uint8;
enum class ECamperMoveState : uint8;
enum class ECamperStanceState : uint8;
/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API UCamperAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	// Camper 저장 변수
	UPROPERTY(BlueprintReadOnly)
	class ACamper* camper;
	
	// 생존자 자세 상태 애니메이션 스테이트
	UPROPERTY(BlueprintReadOnly, Category = "AnimFSM")
	ECamperStanceState animCamperStanceState;

	// 생존자 움직임 상태 애니메이션 스테이트
	UPROPERTY(BlueprintReadOnly, Category = "AnimFSM")
	ECamperMoveState animCamperMoveState;
	
	// 건강 상태 애니메이션 스테이트
	UPROPERTY(BlueprintReadOnly, Category = "AnimFSM")
	ECamperHealth animHealthState;

	// 생존자 상호작용 스테이트
	UPROPERTY(BlueprintReadOnly, Category = "AnimFSM")
	ECamperInteraction animCamperInteractionState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	float moveSpeed; // 현재 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bWalk = false; // 일반 걷기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bRun = false; // 일반 뛰기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bCrouch = false; // 앉기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bStartRepair = false; // 수리 시작 했는지 체크 하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bEndRepair = true; // 수리가 끝났는지 체크 하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bInjure = false; // 다친 상태 판별 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bSelfHealing = false; // 자가 치유 시작하고 끝내는 조건 판별 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bCrawl = false; // 피가 0이 됐을 때 기어다니기 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bHitCrawl = false; // 피가 0이 됐을 때 기어다니기 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bUnLock = false; // 문을 열었는지 안열열었는지 확인 하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bUnLocking = false; // 문을 열고 있는 중인지 확인하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	int32 randValue = 0; // 앉아서 걷는 애니메이션 선택 하는 값
	
	void IsWalk(); // 걷고 있는지 체크
	void IsRun(); // 뛰고 있는지 체크
	void IsCrouch(bool value); // 앉아 있는지 체크
	int32 RandomValue(); // 1, 2중 값 반환

	// 기어가는 불 값 변경하는 함수 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayHitCrawlAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayHitCrawlAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PlayHitCrawlAnimation(FName sectionName);
	// 발전기 고치는 애니메이션 몽타주 실행하는 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayRepairAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayRepairAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PlayRepairAnimation(FName sectionName);
	// 데드하드퍽 애니메이션 몽타주 실행하는 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayDeadHardAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayDeadHardAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PlayDeadHardAnimation(FName sectionName);
	// 자가치유퍽 애니메이션 몽타주 실행하는 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlaySelfHealingAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlaySelfHealingAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PlaySelfHealingAnimation(FName sectionName);
	// 문여는 애니메이션 몽타주 실행하는 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayUnLockAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayUnLockAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PlayUnLockAnimation(FName sectionName);
	// 갈고리 애니메이션 몽타주 실행하는 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayHookingAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayHookingAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PlayHookingAnimation(FName sectionName);
	// 갈고리에서 구해주는 애니메이션 몽타주 실행하는 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayRescueHookingAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayRescueHookingAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PlayRescueHookingAnimation(FName sectionName);
	
	UFUNCTION()
	void AnimNotify_StartGen();
	UFUNCTION()
	void AnimNotify_GenEnd();

	UFUNCTION()
	void AnimNotify_StartSelfHealing();
	UFUNCTION()
	void AnimNotify_EndSelfHealing();
	UFUNCTION()
	void AnimNotify_StartUnLock();
	UFUNCTION()
	void AnimNotify_EndUnLock();

	UFUNCTION()
	void AnimNotify_LeftFoot();
	UFUNCTION()
	void AnimNotify_RightFoot();
	UFUNCTION()
	void AnimNotify_InjureLeftFoot();
	UFUNCTION()
	void AnimNotify_InjureRightFoot();
};
