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
	OnStopInteraction.Broadcast(this, OtherActor);
}

void UInteractionPoint::AttachActor(AActor* Actor)
{
	Actor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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