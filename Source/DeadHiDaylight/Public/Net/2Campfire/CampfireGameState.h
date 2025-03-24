// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CampfireGameState.generated.h"

enum class EPlayerRole : uint8;
/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ACampfireGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	TMap<FGuid, EPlayerRole> RoleMap;
	
	UPROPERTY(ReplicatedUsing = OnRep_SlotCount)
	int SlasherCount = 0;
	UPROPERTY(ReplicatedUsing = OnRep_SlotCount)
	int CamperCount = 0;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestSelect(class ACampfirePlayerController* Controller, const EPlayerRole ReqRole);

	UFUNCTION()
	void OnRep_SlotCount() const;
};
