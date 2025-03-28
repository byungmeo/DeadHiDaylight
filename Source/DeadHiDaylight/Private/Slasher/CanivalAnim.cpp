// Fill out your copyright notice in the Description page of Project Settings.


#include "CanivalAnim.h"

#include "Canival.h"
#include "KismetAnimationLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCanivalAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Canival = Cast<ACanival>(GetOwningActor());
}

void UCanivalAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Canival != nullptr)
	{
		FVector Velocity = Canival->GetVelocity();
		Speed = Velocity.Size2D();
		
		bIsMoving = (Speed > 0.1f);
		UE_LOG(LogTemp, Warning, TEXT("%f"), Speed);
		
		bIsInAir = Canival->GetMovementComponent()->IsFalling();

		CamRotator = Canival->Camera->GetComponentRotation();

		FRotator ActorRotation = Canival->GetActorRotation();
		float Dir = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);
		Direction = FMath::RoundToInt(Dir);


		//
		// float V = UKismetMathLibrary::VSize(Canival->GetVelocity());
		// bIsMoving = (V > 0);
		//
		// bIsInAir = Canival->GetMovementComponent()->IsFalling();
		//
		// CamRotator = Canival->Camera->GetComponentRotation();
		//
		// FVector Velocity = Canival->GetVelocity();
		// Speed = Velocity.Size();
		//
		// // 방향 계산
		// FRotator Rotation = Canival->GetActorRotation();
		// FVector ForwardVector = Rotation.Vector();  // 캐릭터의 정면 방향
		// FVector RightVector = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);  // 캐릭터의 오른쪽 방향
		//
		// // 방향 값 계산 (Dot Product)
		// float ForwardAmount = FVector::DotProduct(Velocity, ForwardVector);
		// float RightAmount = FVector::DotProduct(Velocity, RightVector);
		//
		//
		// auto ownerPawn = TryGetPawnOwner();
		// auto player = Cast<ACanival>(ownerPawn);
		// if (player)
		// {
		// 	// walkSpeed 값을 설정
		// 	FVector velocity = player->GetVelocity();
		// 	// 전방벡터
		// 	FVector forward = player->GetActorForwardVector();
		// 	player = FVector::DotProduct(velocity, forward);
		//
		// 	// 좌우방향
		// 	FVector right = player->GetActorRightVector();
		// 	player = FVector::DotProduct(velocity, right);
		// }
		//
		// //
		// // 방향 값 설정
		// if (ForwardAmount > 0)
		// 	Direction = 0;  // 전진
		// else if (ForwardAmount < 0)
		// 	Direction = 180;  // 후진
		//
		// if (RightAmount > 0)
		// 	Direction = 90;  // 우측
		// else if (RightAmount < 0)
		// 	Direction = -90;  // 좌측

			
		/*NewCamBoneRotation = Canival->
		auto Rot = NewCamBoneTransform.GetRotation();
		Rot.Z += LookVector.X;
		Rot.Y += LookVector.Y;
		NewCamBoneTransform.SetRotation(Rot);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *NewCamBoneTransform.ToString());*/
	}
}



void UCanivalAnim::CompletedKickAnimation()
{
	// Canival->CompletedKick();
}
