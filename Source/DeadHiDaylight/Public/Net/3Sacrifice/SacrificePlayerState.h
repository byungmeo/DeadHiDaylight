// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DHDGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "SacrificePlayerState.generated.h"

enum class EPlayerRole : uint8;

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
	ECH_Crawl		UMETA(DisplayName = "Crawl"),
	ECH_Carry		UMETA(DisplayName = "Carry"),
	ECH_Hook		UMETA(DisplayName = "Hook"),
	ECH_Dead		UMETA(DisplayName = "Dead")
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
	EStrugglePhase StrugglePhase = EStrugglePhase::ESP_First;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECamperHealth Health = ECamperHealth::ECH_Healthy;
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
