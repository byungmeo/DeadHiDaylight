// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperComps/CamperFSM.h"

#include "CamperAnimInstance.h"


// Sets default values for this component's properties
UCamperFSM::UCamperFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCamperFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (auto* meshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		anim = Cast<UCamperAnimInstance>(meshComp->GetAnimInstance());
	}
}


// Called every frame
void UCamperFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FString stateStr = UEnum::GetValueAsString(curHealthState);
	FString stateStr = UEnum::GetValueAsString(curState);
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, *stateStr);


	SetCamperState(curState); // 상태 스테이트
	SetCamperHealthState(curHealthState); // 건강 스테이트
	SetCamperInteractionState(curInteractionState); // 상호작용 스테이트
}

void UCamperFSM::SetCamperState(ECamperState newState)
{
	switch (newState)
	{
	case ECamperState::ECS_Idle:
		IdleState();
		break;
	case ECamperState::ECS_Move:
		MoveState();
		break;
	case ECamperState::ECS_Run:
		RunState();
		break;
	case ECamperState::ECS_Crouch:
		CrouchState();
		break;
	default:
		break;
	}
}

void UCamperFSM::SetCamperHealthState(ECamperHealth newHealthState)
{
	switch (newHealthState)
	{
	case ECamperHealth::ECH_Healthy:
		HealthyState();
		break;
	case ECamperHealth::ECH_Injury:
		InjureyState();
		break;
	case ECamperHealth::ECH_Crawl:
		CrawlState();
		break;
	case ECamperHealth::ECH_Dead:
		DeadState();
		break;
	default:
		break;
	}
}

void UCamperFSM::SetCamperInteractionState(ECamperInteraction newInteractionState)
{
	switch (curInteractionState)
	{
	case ECamperInteraction::ECI_NONE:
		NONEState();
		break;
	case ECamperInteraction::ECI_Repair:
		RepairState();
		break;
	case ECamperInteraction::ECI_DeadHard:
		DeadHardState();
		break;
	case ECamperInteraction::ECI_SelfHealing:
		SelfHealingState();
		break;
	case ECamperInteraction::ECI_Carry:
		CarryState();
		break;
	case ECamperInteraction::ECI_Hook:
		HookState();
		break;
	case ECamperInteraction::ECI_HookRescue:
		HookRescueState();
		break;
	case ECamperInteraction::ECI_UnLock:
		UnLookState();
		break;
	default:
		break;
	}
}

void UCamperFSM::IdleState()
{
	if (anim == nullptr) return;
	anim->animCamperState = ECamperState::ECS_Idle;
}

void UCamperFSM::MoveState()
{
	if (anim == nullptr) return;
	anim->animCamperState = ECamperState::ECS_Move;
}

void UCamperFSM::RunState()
{
	if (anim == nullptr) return;
	anim->animCamperState = ECamperState::ECS_Run;
}

void UCamperFSM::CrouchState()
{
	if (anim == nullptr) return;
	anim->animCamperState = ECamperState::ECS_Crouch;
}

// 건강한 상태
void UCamperFSM::HealthyState()
{
	anim->animHealthState = ECamperHealth::ECH_Healthy;
	
}
// 다친 상태
void UCamperFSM::InjureyState()
{
	anim->animHealthState = ECamperHealth::ECH_Injury;
}
// 기어다니는 상태
void UCamperFSM::CrawlState()
{
	anim->animHealthState = ECamperHealth::ECH_Crawl;
}
// 죽은 상태
void UCamperFSM::DeadState()
{
	anim->animHealthState = ECamperHealth::ECH_Dead;
}


void UCamperFSM::NONEState()
{
	anim->animCamperInteractionState = ECamperInteraction::ECI_NONE;
}

void UCamperFSM::RepairState()
{
	anim->animCamperInteractionState =	ECamperInteraction::ECI_Repair;
}

void UCamperFSM::DeadHardState()
{
	anim->animCamperInteractionState =	ECamperInteraction::ECI_DeadHard;
}

void UCamperFSM::SelfHealingState()
{
	anim->animCamperInteractionState = ECamperInteraction::ECI_SelfHealing;	
}
// 살인마한테 얹힌 상태
void UCamperFSM::CarryState()
{
	anim->animCamperInteractionState = ECamperInteraction::ECI_Carry;
}
// 갈고리에 걸린 상태
void UCamperFSM::HookState()
{
	anim->animCamperInteractionState = ECamperInteraction::ECI_Hook;
}
void UCamperFSM::HookRescueState()
{
	anim->animCamperInteractionState = ECamperInteraction::ECI_HookRescue;
}
void UCamperFSM::UnLookState()
{
	anim->animCamperInteractionState = ECamperInteraction::ECI_UnLock;	
}








