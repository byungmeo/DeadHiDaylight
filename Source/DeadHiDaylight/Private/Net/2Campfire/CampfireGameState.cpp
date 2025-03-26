// Fill out your copyright notice in the Description page of Project Settings.


#include "CampfireGameState.h"

#include "CampfirePlayerController.h"
#include "DHDGameInstance.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Net/UnrealNetwork.h"

void ACampfireGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACampfireGameState, SlasherCount);
	DOREPLIFETIME(ACampfireGameState, CamperCount);
}

void ACampfireGameState::ServerRPC_RequestSelect_Implementation(ACampfirePlayerController* Controller, const EPlayerRole ReqRole)
{
	const FGuid Guid = Controller->Guid;
	
	if (false == RoleMap.Contains(Guid))
	{
		NET_LOG(LogTemp, Warning, TEXT("Unknown Guid [%s]"), *Guid.ToString());
		return;
	}

	if (ReqRole == EPlayerRole::EPR_None || ReqRole == EPlayerRole::EPR_Observer)
	{
		NET_LOG(LogTemp, Warning, TEXT("[%s] RequestSelect %s. but, This role is not selectable"), *Guid.ToString(), *UEnum::GetValueAsString(ReqRole));
		return;
	}

	if (RoleMap[Guid] == ReqRole)
	{
		NET_LOG(LogTemp, Warning, TEXT("[%s] RequestSelect %s. but, already selected"), *Guid.ToString(), *UEnum::GetValueAsString(ReqRole));
		return;
	}
	
	if (ReqRole == EPlayerRole::EPR_Slasher)
	{
		if (SlasherCount >= 1)
		{
			NET_LOG(LogTemp, Warning, TEXT("[%s] RequestSelect %s. but, slot is full"), *Guid.ToString(), *UEnum::GetValueAsString(ReqRole));
			return;
		}
		if (Controller->SelectedRole == EPlayerRole::EPR_Camper)
		{
			CamperCount--;
		}
		SlasherCount++;
	}
	else if (ReqRole == EPlayerRole::EPR_Camper)
	{
		if (CamperCount >= 4)
        {
        	NET_LOG(LogTemp, Warning, TEXT("[%s] RequestSelect %s. but, slot is full"), *Guid.ToString(), *UEnum::GetValueAsString(ReqRole));
			return;
        }
		if (Controller->SelectedRole == EPlayerRole::EPR_Slasher)
		{
			SlasherCount--;
		}
		CamperCount++;
	}

	Controller->SelectedRole = ReqRole;
	RoleMap[Guid] = ReqRole;
}

void ACampfireGameState::OnRep_SlotCount() const
{
	ACampfirePlayerController* PlayerController = Cast<ACampfirePlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->UpdateSlot(SlasherCount, CamperCount);
	}
}
