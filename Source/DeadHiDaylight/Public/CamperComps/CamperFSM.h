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
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	ECamperState curState = ECamperState::ECS_Idle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	ECamperHealth curHealthState = ECamperHealth::ECH_Healthy;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	ECamperInteraction curInteractionState = ECamperInteraction::ECI_NONE;
	
	UPROPERTY()
	class UCamperAnimInstance* anim;

	// 이동 상태 함수
	void SetCamperState(ECamperState newState);
	void IdleState();
	void MoveState();
	void RunState();
	void CrouchState();
	
	// 건강 상태 함수
	void SetCamperHealthState(ECamperHealth newHealthState);
	void HealthyState();
	void InjureyState();
	void CrawlState();
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
