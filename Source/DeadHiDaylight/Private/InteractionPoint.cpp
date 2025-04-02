// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionPoint.h"

#include "Canival.h"
#include "Generator.h"
#include "MeatHook.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Camper.h"


// Sets default values for this component's properties
UInteractionPoint::UInteractionPoint()
{
	PrimaryComponentTick.bCanEverTick = true;
	BoxExtent = FVector(1, 1, 1);
}

UInteractionPoint* UInteractionPoint::FindInteractionPoint(const AActor* Actor, const FVector& Start, const FVector& End, const EInteractionMode FindMode)
{
	if (FindMode == EInteractionMode::EIM_None || FindMode == EInteractionMode::EIM_Both)
	{
		UE_LOG(LogTemp, Error, TEXT("UInteractionPoint::FindInteractionPoint INVALID FindMode"));
		return nullptr;
	}
	
	TArray<AActor*> ActorsToIgnore;
    TArray<FHitResult> OutHits;
    const bool bHit = UKismetSystemLibrary::SphereTraceMultiByProfile(
    	Actor->GetWorld(),
    	Start,
    	End,
    	500,
    	TEXT("InteractionPoint"),
    	false,
    	ActorsToIgnore,
    	EDrawDebugTrace::ForOneFrame,
    	OutHits,
    	false
    );
	
	if (bHit)
	{
		for (int i = 0; i < OutHits.Num(); i++)
		{
			// 상호작용 불가능 한 Point는 거른다
			const auto* Point = Cast<UInteractionPoint>(OutHits[i].Component);
			if (false == Point->bCanInteract || (Point->InteractionMode != FindMode && Point->InteractionMode != EInteractionMode::EIM_Both))
			{
				OutHits.RemoveAt(i);
				i--;
				continue;
			}
			if (false == Point->IsCanInteractionWithThisActor(Actor))
			{
				OutHits.RemoveAt(i);
				i--;
				continue;
			}
			OutHits[i].Distance = FVector::Dist(Point->GetComponentLocation(), Start);
			// TODO: 바라보는 방향 고려
		}
	}
	
	if (OutHits.Num() > 0)
	{
		// 가장 거리가 가까운 Point를 반환
		Algo::SortBy(OutHits, &FHitResult::Distance);
		return Cast<UInteractionPoint>(OutHits[0].Component);
	}
	return nullptr;
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
	
	if (false == GetOwner()->HasAuthority())
	{
		return;
	}
	
	if (bSkillCheckEnable && AttachedActor)
	{
		// SCREEN_LOG(TEXT("SkillCheckTick"));
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

bool UInteractionPoint::IsCanInteractionWithThisActor(const AActor* Actor) const
{
	auto* Owner = GetOwner();
	auto* Camper = Cast<ACamper>(Actor);
	auto* Slasher = Cast<ACanival>(Actor);
	if (auto* Generator = Cast<AGenerator>(Owner))
	{
		if (Slasher)
		{
			if (Generator->bIsBreak)
			{
				return false;
			}
		}
	}
	else if (auto* MeatHook = Cast<AMeatHook>(Owner))
	{
		if (Slasher)
		{
			if (nullptr == Slasher->AttachedSurvivor)
			{
				return false;
			}
		}
	}

	return true;
}
