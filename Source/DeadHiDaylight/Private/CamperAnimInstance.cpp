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
	UE_LOG(LogTemp, Warning, TEXT("UCamperAnimInstance::IsWalk %d"), bWalk);
}

void UCamperAnimInstance::IsRun()
{
	bRun = !bRun;
	UE_LOG(LogTemp, Warning, TEXT("UCamperAnimInstance::IsRun %d"), bRun);
}
