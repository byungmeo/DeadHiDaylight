// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperAnimInstance.h"

#include "Player/Camper.h"

void UCamperAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	camper = Cast<ACamper>(GetOwningActor());
}

void UCamperAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (auto* player = Cast<ACamper>(TryGetPawnOwner()))
	{
		// 전방방향 벡터 구하기
		FVector velocity = player->GetVelocity();
		FVector forward = player->GetActorForwardVector();
		moveSpeed = FVector::DotProduct(velocity, forward);
	}
}

// 기어가는 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_PlayHitCrawlAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlayHitCrawlAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlayHitCrawlAnimation_Implementation(FName sectionName)
{
	PlayHitCrawlAnimation(sectionName);
}
// 발전기 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_PlayRepairAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlayRepairAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlayRepairAnimation_Implementation(FName sectionName)
{
	PlayRepairAnimation(sectionName);
}
// 데드하드퍽 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_PlayDeadHardAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlayDeadHardAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlayDeadHardAnimation_Implementation(FName sectionName)
{
	PlayDeadHardAnimation(sectionName);
}
// 자가치유퍽 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_PlaySelfHealingAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlaySelfHealingAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlaySelfHealingAnimation_Implementation(FName sectionName)
{
	PlaySelfHealingAnimation(sectionName);
}
// 문여는 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_PlayUnLockAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlayUnLockAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlayUnLockAnimation_Implementation(FName sectionName)
{
	PlayUnLockAnimation(sectionName);
}
// 갈고리 거는 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_PlayHookingAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlayHookingAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlayHookingAnimation_Implementation(FName sectionName)
{
	PlayHookingAnimation(sectionName);
}
// 갈고리에서 구해주는 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_PlayRescueHookingAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlayRescueHookingAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlayRescueHookingAnimation_Implementation(FName sectionName)
{
	PlayRescueHookingAnimation(sectionName);
}

// 살인마가 생존자를 들거나 떨어트리는 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_PickUpAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PickUpAnimation(sectionName);
}
void UCamperAnimInstance::MultiCastRPC_PickUpAnimation_Implementation(FName sectionName)
{
	PlayPickUpAnimation(sectionName);
}
// 판자 내리는 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_PullDownPalletAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PullDownPalletAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PullDownPalletAnimation_Implementation(FName sectionName)
{
	PlayPullDownPalletAnimation(sectionName);
}
// 상대 치유 하는 애니메이션 RPC
void UCamperAnimInstance::ServerRPC_HealingAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_HealingAnimation(sectionName);
}
void UCamperAnimInstance::MultiCastRPC_HealingAnimation_Implementation(FName sectionName)
{
	PlayHealingAnimation(sectionName);
}

void UCamperAnimInstance::AnimNotify_LeftFoot()
{
	camper->PlayLeftSound();
}

void UCamperAnimInstance::AnimNotify_RightFoot()
{
	camper->PlayRightSound();
}

void UCamperAnimInstance::AnimNotify_InjureLeftFoot()
{
	camper->PlayLeftSound();
}

void UCamperAnimInstance::AnimNotify_InjureRightFoot()
{
	camper->PlayRightSound();
}

