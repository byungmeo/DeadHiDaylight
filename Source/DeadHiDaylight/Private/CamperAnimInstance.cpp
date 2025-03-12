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
		dirV = FVector::DotProduct(velocity, forward);
		// 좌우방향 벡터 구하기
		FVector right = player->GetActorRightVector();
		dirH = FVector::DotProduct(velocity, right);
	}
}
