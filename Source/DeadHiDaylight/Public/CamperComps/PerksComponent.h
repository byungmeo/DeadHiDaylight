// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CamperAnimInstance.h"
#include "CamperComps/CamperBaseComponent.h"
#include "PerksComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADHIDAYLIGHT_API UPerksComponent : public UCamperBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPerksComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* pi) override;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_DeadHard;
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_SelfHealing;

	// anim 저장용 변수
	UCamperAnimInstance* anim;
	
	AActor* owner;
	
	// 데드하드 사용 가능한지 판단하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Perks")
	bool bDeadHard = false;
	// 탈진 시간 체크하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Perks")
	float exTime = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Perks")
	float exhaustionTime = 40;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Perks")
	float healingTime = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Perks")
	float selfhealingTime = 32;
	
	// 데드하드퍽 바인드용 함수
	void PerksDeadHard();
	UFUNCTION(Server, Reliable)
	void ServerRPC_PerksDeadHard();
	UFUNCTION(NetMulticast, Reliable)
	void NetMultiCastRPC_PerksDeadHard();
	// 데드하드 탈진 상태 체크 함수
	void DeadHardTimingCheck(float deltaTime);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PerksDeadHardTimingCheck(bool value);
	UFUNCTION(NetMulticast, Reliable)
	void NetMultiCastRPC_PerksDeadHardTimingCheck(bool value);
	// 자가치유퍽 시작 함수
	void PerksSelfHealing();
	UFUNCTION(Server, Reliable)
	void ServerRPC_PerksSelfHealing();
	UFUNCTION(NetMulticast, Reliable)
	void NetMultiCastRPC_PerksSelfHealing();
	// 자가치유퍽 끝 함수
	void StopPerksSelfHealing();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopPerSelfHealing();
	UFUNCTION(NetMulticast, Reliable)
	void NetMultiCastRPC_StopPerSelfHealing();
	
	void SelfHealingTimingCheck(float deltaTime);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SelfHealingTimingCheck(bool value);
	UFUNCTION(NetMulticast, Reliable)
	void NetMultiCastRPC_SelfHealingTimingCheck(bool value);
};
