// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeGameState.h"

#include "DHDGameInstance.h"
#include "Generator.h"
#include "SacrificeGameMode.h"
#include "SacrificePlayerState.h"
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
	ReqGeneratorCount = OutActors.Num();

	if (HasAuthority())
	{
		const auto* GameInstance = Cast<UDHDGameInstance>(GetGameInstance());
		RemCamperCount = GameInstance->RoleMap.Num() - 2;
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

void ASacrificeGameState::ServerOnly_OnCamperOutGame(ASacrificePlayerState* State)
{
	if (--RemCamperCount <= 0)
	{
		ServerOnly_GameEnd();
	} 
}

void ASacrificeGameState::ServerOnly_GameEnd()
{
	TMap<FString, int> MatchResultMap;
	for (auto PlayerState : PlayerArray)
	{
		if (auto* SacrificePlayerState = Cast<ASacrificePlayerState>(PlayerState))
		{
			auto CamperState = SacrificePlayerState->UserState;
			if (CamperState.PlayerRole != EPlayerRole::EPR_Camper)
			{
				continue;
			}

			// TODO: 마지막 생존자 상태에 따라 숫자 부여
			// 0 : 생존 (게임이 완전히 끝나기 전에 탈출 또는 사망하여 해당 시점에 생존해있는 다른 생존자)\n
			// 1 : 탈출\n
			// 2 : 사망
			// 이외 모두 생존 처리
		}
	}
	// TODO: MatchResultMap을 Multicast로 각자의 Instance에 때려박도록 하자
}