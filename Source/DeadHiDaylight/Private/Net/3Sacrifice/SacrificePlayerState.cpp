// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificePlayerState.h"

#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Net/UnrealNetwork.h"

void ASacrificePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASacrificePlayerState, UserState);
}

void ASacrificePlayerState::OnRep_UserState()
{
	NET_LOG(LogTemp, Warning, TEXT("ASacrificePlayerState::OnRep_UserState"));
	OnUpdatedUserState.Broadcast(UserState);
}

void ASacrificePlayerState::ChangeHealth()
{
	FUserState NewState = UserState;
    NewState.Health = ECamperHealth::ECH_Injury;
    UserState = NewState;
	if (HasAuthority())
	{
		OnRep_UserState();
	}
}
