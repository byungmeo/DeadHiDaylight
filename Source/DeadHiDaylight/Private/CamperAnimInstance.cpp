// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperAnimInstance.h"

#include "Camper.h"


void UCamperAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (auto player = Cast<ACamper>(TryGetPawnOwner()))
	{
		// 전방방향 벡터 구하기
		FVector velocity = player->GetVelocity();
		FVector forward = player->GetActorForwardVector();
		moveSpeed = FVector::DotProduct(velocity, forward);
	}
}

void UCamperAnimInstance::IsWalk()
{
	bWalk = !bWalk;
	// UE_LOG(LogTemp, Warning, TEXT("UCamperAnimInstance::IsWalk %d"), bWalk);
}

void UCamperAnimInstance::IsRun()
{
	bRun = !bRun;
	// UE_LOG(LogTemp, Warning, TEXT("UCamperAnimInstance::IsRun %d"), bRun);
}

void UCamperAnimInstance::IsCrouch(bool value)
{
	bCrouch = value;
	randValue = RandomValue();
	UE_LOG(LogTemp, Warning, TEXT("UCamperAnimInstance::bCrouch %d randomValue : %d"), bCrouch, randValue);
}

int32 UCamperAnimInstance::RandomValue()
{
	return FMath::RandRange(1, 2);
}

void UCamperAnimInstance::AnimNotify_StartGen()
{
	bStartRepair = true;
	bEndRepair = true;
}

void UCamperAnimInstance::AnimNotify_GenEnd()
{
	bStartRepair = false;
	Montage_Stop(0.2f);
}

void UCamperAnimInstance::AnimNotify_StartSelfHealing()
{
	bSelfHealing = true;
}

void UCamperAnimInstance::AnimNotify_EndSelfHealing()
{
	bSelfHealing = false;
	Montage_Stop(0.2f);
}

void UCamperAnimInstance::AnimNotify_StartUnLock()
{
	bUnLocking = true;
}

void UCamperAnimInstance::AnimNotify_EndUnLock()
{
	bUnLocking = false;
	bUnLock = true;
	Montage_Stop(0.2f);
}

void UCamperAnimInstance::ServerRPC_HitCrawl_Implementation()
{
	MultiCastRPC_HitCrawl();
}

void UCamperAnimInstance::MultiCastRPC_HitCrawl_Implementation()
{
	HitCrawl();
}

void UCamperAnimInstance::ServerRPC_PlayRepairAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlayRepairAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlayRepairAnimation_Implementation(FName sectionName)
{
	PlayRepairAnimation(sectionName);
}

void UCamperAnimInstance::ServerRPC_PlayDeadHardAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlayDeadHardAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlayDeadHardAnimation_Implementation(FName sectionName)
{
	PlayDeadHardAnimation(sectionName);
}

void UCamperAnimInstance::ServerRPC_PlaySelfHealingAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlaySelfHealingAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlaySelfHealingAnimation_Implementation(FName sectionName)
{
	PlaySelfHealingAnimation(sectionName);
}

void UCamperAnimInstance::ServerRPC_PlayUnLockAnimation_Implementation(FName sectionName)
{
	MultiCastRPC_PlayUnLockAnimation(sectionName);
}

void UCamperAnimInstance::MultiCastRPC_PlayUnLockAnimation_Implementation(FName sectionName)
{
	PlayUnLockAnimation(sectionName);
}
