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
	// TODO: GameMode 등을 통해서 전달받도록 변경하기
	TArray<class AGenerator*> Generators;
	
	UPROPERTY(ReplicatedUsing = OnRep_ReqGeneratorCount)
	int ReqGeneratorCount = 5;

	UFUNCTION()
	void OnPowerOn();
	
	UFUNCTION()
    void OnRep_ReqGeneratorCount() const;
	FOnRepGeneratorCount OnRepGeneratorCount;
};
