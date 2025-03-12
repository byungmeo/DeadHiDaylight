// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camper.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPowerOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplosion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBreak);

UCLASS()
class DEADHIDAYLIGHT_API AGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 살인마는 수리가 진행 중인 발전기에 손상을 입힐 수 있으며,
	// 손상된 발전기는 즉시 5%의 진행도를 잃어버리고 초당 0.25%의 진행도가 깎인다.
	// 생존자가 5%만큼 다시 수리를 재개하면 수리 진행도 감소가 정지된다.
	float ImmediateBreakValue = 0.05f;
	float ProgressiveBreakValue = 0.0025f;
	float InitBreakShield = 0.05f;
	float RemainBreakShield = 0;
	
	// 1명 기준 초당 1.25% 수리할 수 있음
	// 인원 수가 늘어날 때마다 기본 효율의 10% 수치만큼 인당 수리 효율이 깎임
	float BaseRepairValue = 0.0125f;
	float ReductionRepairEfficiency = 0.1f;

	// 발전기 수리 시 각 생존자는 각자 랜덤한 주기로 스킬 체크를 해야 한다.
	// 스킬체크 대성공 시 즉시 1%의 수리 보너스를 얻음.
	// 실패하면 발전기가 폭발하면서 즉시 10%만큼의 진행도가 깎이고, 3초간 수리가 불가능 하다.
	float GreateSuccessBonus = 0.01f;
	float ExplosionDuration = 3.0f;
	float RemainExplosionTime = 0;
	float ImmediateExplosionValue = 0.1f;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UGeneratorRepairSlot> SlotLeft = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UGeneratorRepairSlot> SlotRight = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UGeneratorRepairSlot> SlotFront = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UGeneratorRepairSlot> SlotBack = nullptr;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Test")
	bool bPowerOn = false;
	UPROPERTY(BlueprintAssignable)
	FOnPowerOn OnPowerOn;
	UPROPERTY(EditInstanceOnly, Category="Test")
	int RepairingCount = 0;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Test")
	float PowerGauge = 0.0f;
	void PowerOn();

	/*
	 *	Skill Check
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Test")
	bool bIsExplosion = false;
	void SkillCheckSuccess(const bool bGreateSuccess);
	void SkillCheckFail();
	UPROPERTY(BlueprintAssignable)
	FOnExplosion OnExplosion;
	UFUNCTION(CallInEditor, Category="Test")
    void TestExplosion();

	/*
	 * Break by Slasher
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Test")
	bool bIsBreak = false;
	void Break();
	UPROPERTY(BlueprintAssignable)
	FOnBreak OnBreak;
	UFUNCTION(CallInEditor, Category="Test")
	void TestBreak();

	/*
	 *	
	 */
	void Interact(ACharacter* Character);
	void NotifyStartRepair(ACamper* Camper, UGeneratorRepairSlot* Box);
	void NotifyEndRepair(ACamper* Camper);
	void NotifyStartBreak();
	UGeneratorRepairSlot* FindBoxByCamper(const ACamper* Camper);
	UGeneratorRepairSlot* FindOverlapBox(const ACharacter* Character);
};