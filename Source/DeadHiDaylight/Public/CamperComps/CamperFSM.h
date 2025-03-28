// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CamperBaseComponent.h"
#include "SacrificePlayerState.h"
#include "CamperFSM.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADHIDAYLIGHT_API UCamperFSM : public UCamperBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCamperFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	ECamperHealth curState = ECamperHealth::ECH_Healthy;

	void HealthyState();
	void InjureyState();
	void CrawlState();
	void CarryState();
	void HookState();
	void DeadState();
	
};
