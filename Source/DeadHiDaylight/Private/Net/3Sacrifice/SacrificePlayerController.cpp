// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificePlayerController.h"

#include "SacrificeGameMode.h"
#include "Canival.h"
#include "Camper.h"
#include "DHDGameInstance.h"
#include "SacrificeCommonHUD.h"
#include "SacrificePlayerState.h"
#include "Blueprint/UserWidget.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Kismet/GameplayStatics.h"

void ASacrificePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (HasAuthority())
	{
		InputComponent->BindKey(EKeys::Right, IE_Pressed, this, &ASacrificePlayerController::NextPlayer);
		InputComponent->BindKey(EKeys::Left, IE_Pressed, this, &ASacrificePlayerController::PrevPlayer);
		InputComponent->BindKey(EKeys::F, IE_Pressed, this, &ASacrificePlayerController::ToggleCamera);
	}
}

void ASacrificePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GameMode = Cast<ASacrificeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	}

	SacrificePlayerState = Cast<ASacrificePlayerState>(PlayerState);
	
	if (IsLocalPlayerController())
	{
		ClientRPC_RequestCallbackWithGuid();
		ClientRPC_DisplayHUD();
	}
}

void ASacrificePlayerController::PrevPlayer()
{
	NET_LOG(LogTemp, Warning, TEXT("ASacrificePlayerController::PrevPlayer"));
	if (bIsFreeMode || nullptr == GameMode->Slasher || GameMode->Campers.Num() == 0)
	{
		return;
	}
	
	if (--SpectatorIndex < 0)
	{
		SpectatorIndex = GameMode->Campers.Num();
	}

	if (SpectatorIndex == 0)
	{
		SetViewTargetWithBlend(GameMode->Slasher, 0.5f);
	}
	else
	{
		SetViewTargetWithBlend(GameMode->Campers[SpectatorIndex - 1], 0.5f);
	}
}

void ASacrificePlayerController::NextPlayer()
{
	NET_LOG(LogTemp, Warning, TEXT("ASacrificePlayerController::NextPlayer"));
	if (bIsFreeMode || nullptr == GameMode->Slasher || GameMode->Campers.Num() == 0)
	{
		return;
	}

	if (++SpectatorIndex > GameMode->Campers.Num())
	{
		SpectatorIndex = 0;
	}
	
	if (SpectatorIndex == 0)
	{
		SetViewTargetWithBlend(GameMode->Slasher, 0.5f);
	}
	else
	{
		SetViewTargetWithBlend(GameMode->Campers[SpectatorIndex - 1], 0.5f);
	}
}

void ASacrificePlayerController::ToggleCamera()
{
	NET_LOG(LogTemp, Warning, TEXT("ASacrificePlayerController::ToggleCamera"));
	bIsFreeMode = !bIsFreeMode;

	if (bIsFreeMode)
	{
		if (APawn* ObserverPawn = GetPawn())
		{
			SetViewTargetWithBlend(ObserverPawn, 0.5f);  // 다시 SpectatorPawn 시점으로 전환
		}
	}
	else
	{
		if (nullptr == GameMode->Slasher || GameMode->Campers.Num() == 0)
		{
			return;
		}
		
		if (SpectatorIndex == 0)
		{
			SetViewTargetWithBlend(GameMode->Slasher, 0.5f);
		}
		else
		{
			SetViewTargetWithBlend(GameMode->Campers[SpectatorIndex - 1], 0.5f);
		}
	}
}

void ASacrificePlayerController::ClientRPC_DisplayHUD_Implementation()
{
	Hud = Cast<USacrificeCommonHUD>(CreateWidget(this, HudFactory));
}

void ASacrificePlayerController::ClientRPC_RequestCallbackWithGuid_Implementation()
{
	const auto* ClientGameInstance = Cast<UDHDGameInstance>(GetGameInstance());
	ServerRPC_RequestCreatePawn(ClientGameInstance->Guid);
}

void ASacrificePlayerController::ServerRPC_RequestCreatePawn_Implementation(const FGuid Guid)
{
	NET_LOG(LogTemp, Warning, TEXT("ServerRPC_RequestCreatePawn_Implementation %s"), *Guid.ToString());
	const EPlayerRole PlayerRole = GameMode->ServerGameInstance->RoleMap[Guid];
	GameMode->RequestCreatePawn(this, PlayerRole);
	FBaseState NewState;
	switch (PlayerRole)
	{
	case EPlayerRole::EPR_None:
		{
			NewState = FBaseState();
			break;
		}
	case EPlayerRole::EPR_Observer:
		{
			NewState = FBaseState(FObserverState());
			break;
		}
	case EPlayerRole::EPR_Slasher:
		{
			NewState = FBaseState(FSlasherState());
			break;
		}
	case EPlayerRole::EPR_Camper:
		{
			NewState = FBaseState(FCamperState());
			break;
		}
	}

	NewState.PlayerRole = PlayerRole;
	SacrificePlayerState->PlayerState = NewState;
}

void ASacrificePlayerController::ClientRPC_OnSkillCheck_Implementation(const float Min, const float Max, const float GreatRange)
{
	if (Hud)
    {
		NET_LOG(LogTemp, Warning, TEXT("ClientRPC_OnSkillCheck_Implementation"));
    	Hud->OnSkillCheck(Min, Max, GreatRange);
    }
}