// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperComps/CamperFSM.h"

#include "CamperAnimInstance.h"
#include "SacrificePlayerState.h"
#include "Player/Camper.h"

// Sets default values for this component's properties
UCamperFSM::UCamperFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCamperFSM::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto* meshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		anim = Cast<UCamperAnimInstance>(meshComp->GetAnimInstance());
	}
}

void UCamperFSM::InitializeComponent()
{
	Super::InitializeComponent();

	Camper = Cast<ACamper>(GetOwner());
}


// Called every frame
void UCamperFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetCamperStanceState(curStanceState); // 자세 상태 스테이트
	SetCamperMoveState(curMoveState); // 상태 스테이트
	SetCamperHealthState(curHealthState); // 건강 스테이트
	SetCamperInteractionState(curInteractionState); // 상호작용 스테이트
}
// 자세 업데이트 함수
void UCamperFSM::SetCamperStanceState(ECamperStanceState newState)
{
	switch (newState)
	{
	case ECamperStanceState::ECSS_Idle:
		IdleState();
		break;
	case ECamperStanceState::ECSS_Crouch:
		CrouchState();
		break;
	case ECamperStanceState::ECSS_Crawl:
		CrawlState();
		break;

	default:
		break;
	}
}
// 이동 상태 함수
void UCamperFSM::SetCamperMoveState(ECamperMoveState newState)
{
	switch (newState)
	{
	case ECamperMoveState::ECS_NONE:
		NoneMoveState();
		break;
	case ECamperMoveState::ECS_Move:
		MoveState();
		break;
	case ECamperMoveState::ECS_Run:
		RunState();
		break;
	default:
		break;
	}
}
// 건강 상태 함수
void UCamperFSM::SetCamperHealthState(ECamperHealth newHealthState)
{
	// 서버에서 직접 호출하거나, 클라이언트가 소유권이 있을 때 서버로 요청
	if (GetOwner()->HasAuthority())
	{
		// 서버일 경우 직접 실행
		ServerRPC_SetCamperHealthState(newHealthState);
	}
	else if (Camper && Camper->IsLocallyControlled())
	{
		// 클라이언트에서 소유권 있는 경우 서버로 요청
		ServerRPC_SetCamperHealthState(newHealthState);
	}
}

void UCamperFSM::ServerRPC_SetCamperHealthState_Implementation(ECamperHealth newHealthState)
{
	NetMulticast_SetCamperHealthState(newHealthState);
}

void UCamperFSM::NetMulticast_SetCamperHealthState_Implementation(ECamperHealth newHealthState)
{
	
	switch (newHealthState)
	{
	case ECamperHealth::ECH_Healthy:
		HealthyState();
		break;
	case ECamperHealth::ECH_Injury:
		InjureyState();
		break;
	case ECamperHealth::ECH_Dead:
		DeadState();
		break;
	default:
		break;
	}
}

// 상호작용 상태 함수
void UCamperFSM::SetCamperInteractionState(ECamperInteraction newInteractionState)
{
	switch (newInteractionState)
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
// 기본 상태 함수
void UCamperFSM::IdleState() // 기본 상태 함수
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperStanceState = ECamperStanceState::ECSS_Idle;
}

void UCamperFSM::CrouchState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperStanceState = ECamperStanceState::ECSS_Crouch;
}

void UCamperFSM::CrawlState() // 기어다니는 상태
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperStanceState = ECamperStanceState::ECSS_Crawl;
}

// 이동 상태 함수
void UCamperFSM::NoneMoveState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperMoveState = ECamperMoveState::ECS_NONE;
}

void UCamperFSM::MoveState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperMoveState = ECamperMoveState::ECS_Move;
}

void UCamperFSM::RunState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperMoveState = ECamperMoveState::ECS_Run;
}

// 건강한 상태
void UCamperFSM::HealthyState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animHealthState = ECamperHealth::ECH_Healthy;
}
void UCamperFSM::InjureyState() // 다친 상태
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animHealthState = ECamperHealth::ECH_Injury;
}
void UCamperFSM::DeadState()// 죽은 상태
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animHealthState = ECamperHealth::ECH_Dead;
}

// 상호작용 상태
void UCamperFSM::NONEState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperInteractionState = ECamperInteraction::ECI_NONE;
}
void UCamperFSM::RepairState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperInteractionState =	ECamperInteraction::ECI_Repair;
}
void UCamperFSM::DeadHardState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperInteractionState =	ECamperInteraction::ECI_DeadHard;
}

void UCamperFSM::CarryState() // 살인마한테 얹힌 상태
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperInteractionState = ECamperInteraction::ECI_Carry;
}
void UCamperFSM::HookState() // 갈고리에 걸린 상태
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperInteractionState = ECamperInteraction::ECI_Hook;
}
void UCamperFSM::HookRescueState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperInteractionState = ECamperInteraction::ECI_HookRescue;
}
void UCamperFSM::UnLookState()
{
	if (anim == nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("CamperFSM::IdleState"));
		return;
	}
	anim->animCamperInteractionState = ECamperInteraction::ECI_UnLock;	
}








