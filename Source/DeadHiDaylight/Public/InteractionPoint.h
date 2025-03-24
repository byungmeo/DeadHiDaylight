// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractionPoint.generated.h"

UENUM()
enum class EInteractionMode : uint8
{
	EIM_None,
	EIM_Both,
	EIM_CamperOnly,
	EIM_SlasherOnly
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteraction, UInteractionPoint*, Point, AActor*, OtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStopInteraction, UInteractionPoint*, Point, AActor*, OtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCheck, AActor*, TargetActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADHIDAYLIGHT_API UInteractionPoint : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionPoint();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
public:
	// 현재 상호 작용이 가능한 상태인지 여부
	bool bCanInteract = true;
	// 누구와 상호 작용이 가능한가?
	EInteractionMode InteractionMode = EInteractionMode::EIM_Both;
	
	/**
	 * 상호작용을 시작
	 * @param OtherActor 상호작용을 시도하는 액터
	 */
	void Interaction(AActor* OtherActor);
	FOnInteraction OnInteraction;
	
	/**
	 * 상호작용을 중단
	 * @param OtherActor 상호작용을 중단하는 액터
	 */
	void StopInteraction(AActor* OtherActor);
	FOnStopInteraction OnStopInteraction;

	UPROPERTY()
	TObjectPtr<AActor> AttachedActor = nullptr;
	/**
	 * 액터를 Point에 부착
	 * @param Actor 부착할 액터
	 * @param ForwardOffset
	 * @param bRestoreZ 부착 후 액터의 기존 Z좌표 복구 여부
	 */
	void AttachActor(AActor* Actor, const float ForwardOffset, const bool bRestoreZ);
	void DetachActor();
	
	// 이 InteractionPoint에 Attach된 Actor에 스킬체크가 발동할 수 있는지 여부
	bool bSkillCheckEnable = false;
	// 1초가 지났을 때 스킬 체크가 발동 될 확률
	float SkillCheckChancePerSecond = 0.08f;
	// 스킬체크 쿨타임
	float SkillCheckCooldown = 2.0f;
	float SkillCheckCooldownRemaining = 0.0f;
	FOnSkillCheck OnSkillCheck;
};
