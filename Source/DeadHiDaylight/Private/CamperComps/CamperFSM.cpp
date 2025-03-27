// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperComps/CamperFSM.h"

#include "CamperAnimInstance.h"


// Sets default values for this component's properties
UCamperFSM::UCamperFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCamperFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (auto* meshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		anim = Cast<UCamperAnimInstance>(meshComp->GetAnimInstance());
	}
}


// Called every frame
void UCamperFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString stateStr = UEnum::GetValueAsString(curHealthState);
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, *stateStr);
	
	switch (curHealthState)
	{
		case ECamperHealth::ECH_Healthy:
			HealthyState();
			break;
		case ECamperHealth::ECH_Injury:
			InjureyState();
			break;
		case ECamperHealth::ECH_Crawl:
			CrawlState();
			break;
		case ECamperHealth::ECH_Carry:
			CarryState();
			break;
		case ECamperHealth::ECH_Hook:
			HookState();
			break;
		case ECamperHealth::ECH_Dead:
			DeadState();
			break;
		default:
			break;
	}
}
// 건강한 상태
void UCamperFSM::HealthyState()
{
	anim->animHealthState = ECamperHealth::ECH_Healthy;
	
}
// 다친 상태
void UCamperFSM::InjureyState()
{
	anim->animHealthState = ECamperHealth::ECH_Injury;
}
// 기어다니는 상태
void UCamperFSM::CrawlState()
{
	anim->animHealthState = ECamperHealth::ECH_Crawl;
}
// 살인마한테 얹힌 상태
void UCamperFSM::CarryState()
{
	anim->animHealthState = ECamperHealth::ECH_Carry;
}
// 갈고리에 걸린 상태
void UCamperFSM::HookState()
{
	anim->animHealthState = ECamperHealth::ECH_Hook;
}
// 죽은 상태
void UCamperFSM::DeadState()
{
	anim->animHealthState = ECamperHealth::ECH_Dead;
}











