// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperComps/PerksComponent.h"

#include "Camper.h"
#include "CamperAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	
}


// Called every frame
void UPerksComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPerksComponent::SetupInputBinding(class UEnhancedInputComponent* pi)
{
	Super::SetupInputBinding(pi);

	pi->BindAction(IA_DeadHard, ETriggerEvent::Started, this, &UPerksComponent::PerksDeadHard);
	
}

void UPerksComponent::PerksDeadHard()
{
	if (auto* anim = Camper->Anim)
	{
		//이전 스피드를 담아놓고 데드하드 성공시 1000으로 바꿨다가 1초 뒤 다시 이전 속도로 복귀
		beforeSpeed = Camper->GetCharacterMovement()->MaxWalkSpeed;
		Camper->GetCharacterMovement()->MaxWalkSpeed = 1000;

		// 1초 뒤에 속도 이전 속도로 돌리기
		FTimerHandle deadHardTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(deadHardTimerHandle, this, &UPerksComponent::DeadHardTimer, 1.0f, false);

		// DeadHard 몽타주 실행
		anim->PlayDeadHardAnimation(TEXT("DeadHard"));
	}
}

void UPerksComponent::DeadHardTimer()
{
	Camper->GetCharacterMovement()->MaxWalkSpeed = beforeSpeed;	
}

