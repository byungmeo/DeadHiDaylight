// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SacrificeGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRepGeneratorCount, int, Count);

/**
 * 희생제에서 생존자, 살인마 모두가 가지고 있는(혹은 알아야 하는) 상태
 */
UCLASS()
class DEADHIDAYLIGHT_API ASacrificeGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	TArray<class AGenerator*> Generators;

	/*
	 *	남은 발전기 개수
	 */
	UPROPERTY(ReplicatedUsing = OnRep_ReqGeneratorCount)
	int ReqGeneratorCount = 4;

	/*
	 *	발전기 중 하나가 켜지면 발생하는 이벤트 (서버에서만 로직 실행)
	 */
	UFUNCTION()
	void OnPowerOn();

	/*
	 *	발전기 개수가 변경되면 호출되는 이벤트
	 *	OnRepGeneratorCount.Broadcast() 호출
	 */
	UFUNCTION()
    void OnRep_ReqGeneratorCount();
	FOnRepGeneratorCount OnRepGeneratorCount;
};
