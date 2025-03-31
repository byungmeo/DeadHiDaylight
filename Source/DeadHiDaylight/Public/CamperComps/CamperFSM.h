// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CamperBaseComponent.h"
#include "SacrificePlayerState.h"
#include "CamperFSM.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADHIDAYLIGHT_API UCamperFSM : public UCamperBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCamperFSM();
protected:
	virtual void BeginPlay() override;
public:
	virtual void InitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class UCamperAnimInstance* anim;
	
	// 자세 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	ECamperStanceState curStanceState = ECamperStanceState::ECSS_Idle;
	// 이동 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	ECamperMoveState curMoveState = ECamperMoveState::ECS_NONE;
	// 건강 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	ECamperHealth curHealthState = ECamperHealth::ECH_Healthy;
	// 상호작용 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	ECamperInteraction curInteractionState = ECamperInteraction::ECI_NONE;
	
	// 자세 상태 함수
	void SetCamperStanceState(ECamperStanceState newState);
	void IdleState();
	void CrouchState();
	void CrawlState();
	
	// 이동 상태 함수
	void SetCamperMoveState(ECamperMoveState newState);
	void NoneMoveState();
	void MoveState();
	void RunState();
	
	// 건강 상태 함수
	void SetCamperHealthState(ECamperHealth newHealthState);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetCamperHealthState(ECamperHealth newHealthState);
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetCamperHealthState(ECamperHealth newHealthState);
	void HealthyState();
	void InjureyState();
	void DeadState();

	// 상호작용 함수
	void SetCamperInteractionState(ECamperInteraction newInteractionState);
	void NONEState();
	void RepairState();
	void DeadHardState();
	void SelfHealingState();
	void CarryState();
	void HookState();
	void HookRescueState();
	void UnLookState();
};
