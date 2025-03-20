// Fill out your copyright notice in the Description page of Project Settings.


#include "CanivalAnim.h"

#include "BlendSpaceAnalysis.h"
#include "Canival.h"
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
		float V = UKismetMathLibrary::VSize(Canival->GetVelocity());
		bIsMoving = (V > 0);

		bIsInAir = Canival->GetMovementComponent()->IsFalling();

		CamRotator = Canival->Camera->GetComponentRotation();
		/*NewCamBoneRotation = Canival->
		auto Rot = NewCamBoneTransform.GetRotation();
		Rot.Z += LookVector.X;
		Rot.Y += LookVector.Y;
		NewCamBoneTransform.SetRotation(Rot);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *NewCamBoneTransform.ToString());*/
	}
}
