// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificePlayerState.h"

#include "SacrificePlayerController.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ASacrificePlayerState::BeginPlay()
{
	Super::BeginPlay();
	NET_LOG(LogTemp, Warning, TEXT("ASacrificePlayerState::BeginPlay Role : %d"), UserState.PlayerRole);
}

void ASacrificePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASacrificePlayerState, UserState);
}

void ASacrificePlayerState::OnRep_UserState()
{
	NET_LOG(LogTemp, Warning, TEXT("ASacrificePlayerState::OnRep_UserState Role=%d"), UserState.PlayerRole);
	if (false == bIsInit)
	{
		if (UserState.PlayerRole == EPlayerRole::EPR_Camper)
		{
			auto* LocalPlayerController = Cast<ASacrificePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			LocalPlayerController->OnCreatedNewCamperState(this);
		}
		bIsInit = true;
	}
	OnUpdatedUserState.Broadcast(UserState);
}

void ASacrificePlayerState::ChangeHealth()
{
	FUserState NewState = UserState;
    NewState.Health = (ECamperHealth)(((int)NewState.Health + 1) % 6);
    UserState = NewState;
	OnRep_UserState();
}
