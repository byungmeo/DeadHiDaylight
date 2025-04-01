// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DHDGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "SacrificePlayerState.generated.h"


enum class EPlayerRole : uint8;

/*
 * Camper 자세 상태
 */
UENUM(BlueprintType)
enum class ECamperStanceState : uint8
{
	ECSS_Idle UMETA(DisplayName = "Idle"),
	ECSS_Crawl UMETA(DisplayName = "Crawl"),
	ECSS_Crouch UMETA(DisplayName = "Crouch")
};

/*
 * Camper 이동 상태
 */
UENUM(BlueprintType)
enum class ECamperMoveState : uint8
{
	ECS_NONE UMETA(DisplayName = "NONE"),
	ECS_Move UMETA(DisplayName = "Move"),
	ECS_Run UMETA(DisplayName = "Run")
};

/**
 * 잠식 단계
 */
UENUM(BlueprintType)
enum class EStrugglePhase : uint8
{
	ESP_First		UMETA(DisplayName = "First"),
	ESP_Second		UMETA(DisplayName = "Second"),
	ESP_Third		UMETA(DisplayName = "Third")
};

/**
 * 생존자 건강 상태
 */
UENUM(BlueprintType)
enum class ECamperHealth : uint8
{
	ECH_Healthy		UMETA(DisplayName = "Healthy"),
	ECH_Injury		UMETA(DisplayName = "Injury"),
	ECH_Dead		UMETA(DisplayName = "Dead")
};
/*
 * 생존자 상호작용 상태
 */
UENUM(BlueprintType)
enum class ECamperInteraction : uint8
{
	ECI_NONE		UMETA(DisplayName = "None"),
	ECI_Repair		UMETA(DisplayName = "Repair"),
	ECI_DeadHard		UMETA(DisplayName = "DeadHard"),
	ECI_SelfHealing		UMETA(DisplayName = "SelfHealing"),
	ECI_Carry		UMETA(DisplayName = "Carry"),
	ECI_Hook		UMETA(DisplayName = "Hook"),
	ECI_HookRescue		UMETA(DisplayName = "HookRescue"),
	ECI_UnLock		UMETA(DisplayName = "UnLock")
};


USTRUCT(BlueprintType)
struct FUserState
{
	GENERATED_BODY()

	/*
	 * Common
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName Name = FName(NAME_None);
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EPlayerRole PlayerRole = EPlayerRole::EPR_None;

	/*
	 *	Camper
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECamperStanceState Stance = ECamperStanceState::ECSS_Idle;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECamperMoveState Move = ECamperMoveState::ECS_NONE;	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EStrugglePhase StrugglePhase = EStrugglePhase::ESP_First;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECamperHealth Health = ECamperHealth::ECH_Healthy;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECamperInteraction Interaction = ECamperInteraction::ECI_NONE;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedUserState, const FUserState&, UserState);

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ASacrificePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	bool bIsInit = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_UserState ,BlueprintReadOnly, VisibleAnywhere)
	FUserState UserState;

	UFUNCTION()
	void OnRep_UserState();
	FOnUpdatedUserState OnUpdatedUserState;
	
	UFUNCTION(CallInEditor, Category="Test")
    void ChangeHealth();
};
