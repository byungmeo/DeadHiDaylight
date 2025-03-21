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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADHIDAYLIGHT_API UInteractionPoint : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionPoint();
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
public:
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
};
