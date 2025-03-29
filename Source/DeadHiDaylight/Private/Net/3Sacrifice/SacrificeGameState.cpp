// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeGameState.h"

#include "Generator.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ASacrificeGameState::BeginPlay()
{
	Super::BeginPlay();

	// 모든 발전기 참조를 가져온다.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenerator::StaticClass(), OutActors);
	for (auto* Actor : OutActors)
	{
		AGenerator* Generator = Cast<AGenerator>(Actor);
		Generators.Add(Generator);
		if (HasAuthority())
		{
			Generator->OnPowerOn.AddDynamic(this, &ASacrificeGameState::OnPowerOn);
		}
	}
}

void ASacrificeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASacrificeGameState, ReqGeneratorCount);
}

void ASacrificeGameState::OnPowerOn()
{
	if (false == HasAuthority())
	{
		return;
	}
	ReqGeneratorCount = FMath::Clamp(ReqGeneratorCount - 1, 0, 5);
	NET_LOG(LogTemp, Warning, TEXT("ASacrificeGameState::OnPowerOn : ReqGeneratorCount=%d"), ReqGeneratorCount);
	OnRep_ReqGeneratorCount();
}

void ASacrificeGameState::OnRep_ReqGeneratorCount()
{
	NET_LOG(LogTemp, Warning, TEXT("ASacrificeGameState::OnRep_ReqGeneratorCount = %d"), ReqGeneratorCount);
	OnRepGeneratorCount.Broadcast(ReqGeneratorCount);
}