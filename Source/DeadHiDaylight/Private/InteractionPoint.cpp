// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionPoint.h"

#include "DeadHiDaylight/DeadHiDaylight.h"


// Sets default values for this component's properties
UInteractionPoint::UInteractionPoint()
{
	PrimaryComponentTick.bCanEverTick = true;
	BoxExtent = FVector(1, 1, 1);
}

void UInteractionPoint::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionProfileName(TEXT("InteractionPoint"));
}

void UInteractionPoint::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// TODO:
	/*if (false == GetOwner()->HasAuthority())
	{
		return;
	}*/
	
	if (bSkillCheckEnable && AttachedActor)
	{
		SCREEN_LOG(TEXT("SkillCheckTick"));
		if (SkillCheckCooldownRemaining > 0.0f)
		{
			SkillCheckCooldownRemaining -= DeltaTime;
		}
		else
		{
			if (FMath::RandRange(0.0f, 1.0f) <= SkillCheckChancePerSecond * DeltaTime)
			{
				// 스킬체크는 생존자한테만 일어나지만 InteractionPoint 입장에서는 고려하지 않고, OnSkillCheck를 바인딩 한 쪽에서 관리
				OnSkillCheck.Broadcast(AttachedActor);
				// 스킬체크 중에는 중복으로 발생하지 않도록 함
				SkillCheckCooldownRemaining = SkillCheckCooldown;
			}
		}
	}
}

void UInteractionPoint::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	OnInteraction.Clear();
	OnStopInteraction.Clear();
	OnSkillCheck.Clear();
}

void UInteractionPoint::Interaction(AActor* OtherActor)
{
	OnInteraction.Broadcast(this, OtherActor);
}

void UInteractionPoint::StopInteraction(AActor* OtherActor)
{
	if (nullptr == OtherActor)
	{
		return;
	}
	OnStopInteraction.Broadcast(this, OtherActor);
}

void UInteractionPoint::AttachActor(AActor* Actor, const float ForwardOffset, const bool bRestoreZ)
{
	// 액터의 Z좌표 기억
	const float OrgZ = Actor->GetActorLocation().Z;
	
	Actor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	if (bRestoreZ)
	{
		// 액터의 Z좌표 복구
		FVector NewLocation = Actor->GetActorLocation();
		NewLocation.Z = OrgZ;

		// ForwardOffset 적용
		NewLocation += Actor->GetActorForwardVector() * ForwardOffset;
		
		Actor->SetActorLocation(NewLocation);
	}
	NET_LOG(LogTemp, Warning, TEXT("UInteractionPoint::AttachActor"));
	AttachedActor = Actor;
	bCanInteract = false;
}

void UInteractionPoint::DetachActor()
{
	if (nullptr == AttachedActor)
	{
		return;
	}
	NET_LOG(LogTemp, Warning, TEXT("UInteractionPoint::DetachActor"));
	AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AttachedActor = nullptr;
	bCanInteract = true;
}