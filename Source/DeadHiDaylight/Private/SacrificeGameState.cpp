// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeGameState.h"

#include "Generator.h"
#include "Kismet/GameplayStatics.h"

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
		Generator->OnPowerOn.AddDynamic(this, &ASacrificeGameState::OnPowerOn);
	}
}

void ASacrificeGameState::OnPowerOn()
{
	ReqGeneratorCount--;
	UE_LOG(LogTemp, Display, TEXT("ASacrificeGameState::OnPowerOn : ReqGeneratorCount=%d"), ReqGeneratorCount);

	// TODO: 탈출구 활성화 UI 사운드 재생
}