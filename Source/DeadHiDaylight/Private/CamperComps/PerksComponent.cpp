// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperComps/PerksComponent.h"

#include "Camper.h"
#include "CamperAnimInstance.h"
#include "EnhancedInputComponent.h"


// Sets default values for this component's properties
UPerksComponent::UPerksComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> tempDeadHard(TEXT("/Script/EnhancedInput.InputAction'/Game/JS/Input/IA/IA_DeadHard.IA_DeadHard'"));
	if (tempDeadHard.Succeeded())
	{
		IA_DeadHard = tempDeadHard.Object;
	}
}


// Called when the game starts
void UPerksComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (auto* meshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		anim = Cast<UCamperAnimInstance>(meshComp->GetAnimInstance());
	}
}


// Called every frame
void UPerksComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(anim)
	{
		DeadHardTimingCheck(DeltaTime);
		SelfHealingTimingCheck(DeltaTime);
	}
}

void UPerksComponent::SetupInputBinding(class UEnhancedInputComponent* pi)
{
	Super::SetupInputBinding(pi);

	pi->BindAction(IA_DeadHard, ETriggerEvent::Started, this, &UPerksComponent::PerksDeadHard);
	pi->BindAction(IA_SelfHealing, ETriggerEvent::Started, this, &UPerksComponent::PerksSelfHealing);
	pi->BindAction(IA_SelfHealing, ETriggerEvent::Completed, this, &UPerksComponent::StopPerksSelfHealing);
	
}

void UPerksComponent::PerksDeadHard()
{
	// 데드하드 발동
	if (anim)
	{
		if (anim->bInjure && anim->bRun && bDeadHard == false)
		{
			bDeadHard = true;
			// DeadHard 몽타주 실행
			anim->PlayDeadHardAnimation(TEXT("DeadHard"));
		}
	}
}

void UPerksComponent::DeadHardTimingCheck(float deltaTime)
{
	// 데드하드를 사용하고 뛰지 않을 때만 시간 카운트
	if (bDeadHard && anim->bRun == false)
	{
		// 탈진시간이 exhaustionTime(40초)보다 크거나 같아지면 시간을 초기화하고 데드하드 사용 가능 상태로 변경
		if (exTime >= exhaustionTime)
		{
			exTime = 0;
			bDeadHard = false;
		}
		// 40초 동안 탈진상태
		exTime += deltaTime;
	}
}

void UPerksComponent::PerksSelfHealing()
{
	if (anim == nullptr || Camper->bFindPoints || anim->bSelfHealing || anim->bInjure == false) return;
	UE_LOG(LogTemp, Warning, TEXT("PerksSelfHealing"));

	anim->PlaySelfHealingAnimation(TEXT("StartSelfHealing"));
}

void UPerksComponent::StopPerksSelfHealing()
{
	if (anim == nullptr || Camper->bFindPoints || anim->bSelfHealing == false) return;
	
	anim->PlaySelfHealingAnimation(TEXT("EndSelfHealing"));
}

void UPerksComponent::SelfHealingTimingCheck(float deltaTime)
{
	// 자가치유를 사용할 때
	// 자가치유중이고 다친상태일 때 
	if (anim->bSelfHealing && anim->bInjure)
	{
		// 치유시간이 자가치유 시간을 넘어가면 힐링 시간을 초기화하고 HP를 풀로 채우며 다친 상태를 회복시킨다.
		if (healingTime >= selfhealingTime)
		{
			healingTime = 0;
			Camper->curHP = Camper->maxHP;
			anim->bInjure = false;
		}
		// 32초동안 치유 해야함.
		healingTime += deltaTime;
		UE_LOG(LogTemp, Warning, TEXT("healingTime : %f"), healingTime);
	}
}


