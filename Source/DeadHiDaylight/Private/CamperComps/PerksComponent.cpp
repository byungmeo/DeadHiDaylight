// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperComps/PerksComponent.h"

#include "Player/Camper.h"
#include "CamperAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "SacrificePlayerState.h"
#include "CamperComps/CamperFSM.h"


// Sets default values for this component's properties
UPerksComponent::UPerksComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> tempDeadHard(TEXT("/Script/EnhancedInput.InputAction'/Game/JS/Input/IA/IA_DeadHard.IA_DeadHard'"));
	if (tempDeadHard.Succeeded())
	{
		IA_DeadHard = tempDeadHard.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempSelfHealing(TEXT("/Script/EnhancedInput.InputAction'/Game/JS/Input/IA/IA_SelfHealing.IA_SelfHealing'"));
	if (tempSelfHealing.Succeeded())
	{
		IA_SelfHealing = tempSelfHealing.Object;
	}
	
}


// Called when the game starts
void UPerksComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (auto* meshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		anim = Cast<UCamperAnimInstance>(meshComp->GetAnimInstance());
	}
	if (Camper)
	{
		fsm = Camper->camperFSMComp;
	}
}


// Called every frame
void UPerksComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(anim)
	{
		DeadHardTimingCheck(DeltaTime);
		SelfHealingTimingCheck(DeltaTime);
	}
}

void UPerksComponent::SetupInputBinding(class UEnhancedInputComponent* pi)
{
	Super::SetupInputBinding(pi);

	pi->BindAction(IA_DeadHard, ETriggerEvent::Started, this, &UPerksComponent::PerksDeadHard);
	pi->BindAction(IA_SelfHealing, ETriggerEvent::Started, this, &UPerksComponent::PerksSelfHealing);
	pi->BindAction(IA_SelfHealing, ETriggerEvent::Completed, this, &UPerksComponent::StopPerksSelfHealing);
	
}

void UPerksComponent::PerksDeadHard()
{
	ServerRPC_PerksDeadHard();
}

void UPerksComponent::ServerRPC_PerksDeadHard_Implementation()
{
	NetMultiCastRPC_PerksDeadHard();
}

void UPerksComponent::NetMultiCastRPC_PerksDeadHard_Implementation()
{
	// 데드하드를 사용했고, 다친 상태가 아니고 달리는 상태가 아니면 return
	if (fsm->curInteractionState == ECamperInteraction::ECI_DeadHard ||
		fsm->curHealthState != ECamperHealth::ECH_Injury ||
		fsm->curMoveState != ECamperMoveState::ECS_Run) return;
	
	Camper->SetInteractionState(ECamperInteraction::ECI_DeadHard);
	anim->ServerRPC_PlayDeadHardAnimation(TEXT("DeadHard"));
	// FTimerHandle deadHardHandle;
	// GetWorld()->GetTimerManager().SetTimer(deadHardHandle, [this]()
	// {
	// 	Camper->SetInteractionState(ECamperInteraction::ECI_NONE);
	// }, 0.5f, false);
}

void UPerksComponent::DeadHardTimingCheck(float deltaTime)
{
	// 데드하드를 사용하고 뛰지 않을 때만 시간 카운트
	if (fsm->curInteractionState == ECamperInteraction::ECI_DeadHard &&
		fsm->curMoveState != ECamperMoveState::ECS_Run)
	{
		// 탈진시간이 exhaustionTime(40초)보다 크거나 같아지면 시간을 초기화하고 데드하드 사용 가능 상태로 변경
		if (exTime >= exhaustionTime)
		{
			exTime = 0;
			Camper->SetInteractionState(ECamperInteraction::ECI_NONE);
		}
		// 40초 동안 탈진상태
		exTime += deltaTime;
		UE_LOG(LogTemp, Warning, TEXT("%f"), exTime);
	}
	
}

void UPerksComponent::PerksSelfHealing()
{
	ServerRPC_PerksSelfHealing();
}
void UPerksComponent::ServerRPC_PerksSelfHealing_Implementation()
{
	NetMultiCastRPC_PerksSelfHealing();
}

void UPerksComponent::NetMultiCastRPC_PerksSelfHealing_Implementation()
{
	if (anim == nullptr || Camper->bFindPoints
						|| fsm->curInteractionState == ECamperInteraction::ECI_SelfHealing
						|| fsm->curHealthState != ECamperHealth::ECH_Injury
						|| fsm->curInteractionState == ECamperInteraction::ECI_Repair
						|| fsm->curInteractionState == ECamperInteraction::ECI_UnLock) return;
	
	if (fsm->curStanceState == ECamperStanceState::ECSS_Crouch)
	{
		fsm->curInteractionState = ECamperInteraction::ECI_SelfHealing;
		anim->ServerRPC_PlaySelfHealingAnimation(TEXT("StartCrouchSelfHealing"));
	}
	else if (fsm->curStanceState == ECamperStanceState::ECSS_Crawl)
	{
		fsm->curInteractionState = ECamperInteraction::ECI_SelfHealing;
	}
	else
	{
		fsm->curInteractionState = ECamperInteraction::ECI_SelfHealing;
		anim->ServerRPC_PlaySelfHealingAnimation(TEXT("StartSelfHealing"));
	}
}
void UPerksComponent::StopPerksSelfHealing()
{
	ServerRPC_StopPerSelfHealing();
}

void UPerksComponent::ServerRPC_StopPerSelfHealing_Implementation()
{
	NetMultiCastRPC_StopPerSelfHealing();
}

void UPerksComponent::NetMultiCastRPC_StopPerSelfHealing_Implementation()
{
	if (anim == nullptr || Camper->bFindPoints || fsm->curInteractionState != ECamperInteraction::ECI_SelfHealing) return;

	Camper->StopInjureSound();
	if (fsm->curStanceState == ECamperStanceState::ECSS_Crouch)
	{
		fsm->curInteractionState = ECamperInteraction::ECI_NONE;
		anim->ServerRPC_PlaySelfHealingAnimation(TEXT("EndCrouchSelfHealing"));
	}
	else if (fsm->curStanceState == ECamperStanceState::ECSS_Crawl)
	{
		fsm->curInteractionState = ECamperInteraction::ECI_NONE;
	}
	else
	{
		fsm->curInteractionState = ECamperInteraction::ECI_NONE;
		anim->ServerRPC_PlaySelfHealingAnimation(TEXT("EndSelfHealing"));
	}
}

void UPerksComponent::SelfHealingTimingCheck(float deltaTime)
{
	// 자가치유를 사용할 때
	// 자가치유중이고 다친상태일 때 anim->bSelfHealing && anim->bInjure
	if (fsm->curInteractionState == ECamperInteraction::ECI_SelfHealing &&
		fsm->curHealthState == ECamperHealth::ECH_Injury &&
		fsm->curInteractionState != ECamperInteraction::ECI_Repair &&
		fsm->curInteractionState != ECamperInteraction::ECI_UnLock)
	{
		// 치유시간이 자가치유 시간을 넘어가면 힐링 시간을 초기화하고 HP를 풀로 채우며 다친 상태를 회복시킨다.
		if (healingTime >= selfhealingTime)
		{
			healingTime = 0;
			Camper->curHP = 2;
			Camper->camperFSMComp->curStanceState = ECamperStanceState::ECSS_Idle;
			Camper->camperFSMComp->curHealthState = ECamperHealth::ECH_Healthy;
		}
		// 32초동안 치유 해야함.
		healingTime += deltaTime;
		UE_LOG(LogTemp, Warning, TEXT("healingTime : %f"), healingTime);
	}
}


