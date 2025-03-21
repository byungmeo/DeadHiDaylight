// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionPoint.h"


// Sets default values for this component's properties
UInteractionPoint::UInteractionPoint()
{
	PrimaryComponentTick.bCanEverTick = false;
	BoxExtent = FVector(1, 1, 1);
}

void UInteractionPoint::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionProfileName(TEXT("InteractionPoint"));
}

void UInteractionPoint::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	OnInteraction.Clear();
	OnStopInteraction.Clear();
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

	AttachedActor = Actor;
	SetActive(false);
}

void UInteractionPoint::DetachActor()
{
	if (nullptr == AttachedActor)
	{
		return;
	}
	AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AttachedActor = nullptr;
	SetActive(true);
}