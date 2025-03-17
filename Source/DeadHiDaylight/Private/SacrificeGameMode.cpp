// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeGameMode.h"

#include "Camper.h"
#include "Generator.h"
#include "MeatHook.h"
#include "SacrificeGameState.h"
#include "DeadHiDaylight/Canival.h"
#include "Kismet/GameplayStatics.h"

ASacrificeGameMode::ASacrificeGameMode()
{
	GameStateClass = ASacrificeGameState::StaticClass();
	PlayerStateClass = nullptr;
	DefaultPawnClass = nullptr;
}

void ASacrificeGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACamper::StaticClass(), OutActors);
	for (auto* Actor : OutActors)
	{
		ACamper* Camper = Cast<ACamper>(Actor);
		Campers.Add(Camper);
	}

	Slasher = Cast<ACanival>(UGameplayStatics::GetActorOfClass(GetWorld(), ACanival::StaticClass()));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenerator::StaticClass(), OutActors);
	for (auto* Actor : OutActors)
	{
		AGenerator* Generator = Cast<AGenerator>(Actor);
		Generators.Add(Generator);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMeatHook::StaticClass(), OutActors);
	for (auto* Actor : OutActors)
	{
		AMeatHook* MeatHook = Cast<AMeatHook>(Actor);
		MeatHooks.Add(MeatHook);
	}
}