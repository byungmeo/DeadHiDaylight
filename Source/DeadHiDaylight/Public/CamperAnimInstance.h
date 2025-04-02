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
	// 살인마가 들고 다니는 애니메이션 몽타주 실행하는 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayPickUpAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PickUpAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PickUpAnimation(FName sectionName);
	// 판자 내리는 애니메이션 몽타주 실행하는 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayPullDownPalletAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PullDownPalletAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PullDownPalletAnimation(FName sectionName);
	// 상대 힐링 애니메이션 몽타주 실행하는 RPC
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayHealingAnimation(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_HealingAnimation(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_HealingAnimation(FName sectionName);

	
	UFUNCTION()
	void AnimNotify_LeftFoot();
	UFUNCTION()
	void AnimNotify_RightFoot();
	UFUNCTION()
	void AnimNotify_InjureLeftFoot();
	UFUNCTION()
	void AnimNotify_InjureRightFoot();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayComeHere(bool bIsInjury, bool bIsCrouch);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPointTo(bool bIsInjury, bool bIsCrouch);
};
