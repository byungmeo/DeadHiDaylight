// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificePlayerController.h"

#include "SacrificeGameMode.h"
#include "Canival.h"
#include "Player/Camper.h"
#include "DHDGameInstance.h"
#include "Generator.h"
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

void ASacrificePlayerController::DisplayHud()
{
	Hud = Cast<USacrificeCommonHUD>(CreateWidget(this, HudFactory));
	if (Hud)
	{
		Hud->AddToViewport();
	}
}

void ASacrificePlayerController::OnCreatedNewCamperState(class ASacrificePlayerState* CamperState)
{
	Hud->AddCamperState(CamperState);
}

void ASacrificePlayerController::BeginPlay()
{
	NET_LOG(LogTemp, Warning, TEXT("ASacrificePlayerController::BeginPlay"));
	Super::BeginPlay();
	if (HasAuthority())
	{
		GameMode = Cast<ASacrificeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	}

	LocalGameInstance = Cast<UDHDGameInstance>(GetGameInstance());
	SacrificePlayerState = Cast<ASacrificePlayerState>(PlayerState);
	
	if (IsLocalPlayerController())
	{
		RequestCallbackWithGuid();
		DisplayHud();
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

void ASacrificePlayerController::RequestCallbackWithGuid()
{
	auto* ClientGameInstance = Cast<UDHDGameInstance>(GetGameInstance());
	ServerRPC_RequestCreatePawn(ClientGameInstance->Guid, ClientGameInstance->Nickname);
}

void ASacrificePlayerController::ServerRPC_RequestCreatePawn_Implementation(FGuid Guid, const FText& Nickname)
{
	NET_LOG(LogTemp, Warning, TEXT("ServerRPC_RequestCreatePawn_Implementation %s"), *Guid.ToString());
	const EPlayerRole PlayerRole = GameMode->ServerGameInstance->RoleMap[Guid];
	GameMode->ServerGameInstance->NameMap.Add(Guid, Nickname);
	GameMode->RequestCreatePawn(this, PlayerRole);
	
	FUserState NewState = FUserState();
	NewState.Name = Nickname;
	NewState.PlayerRole = PlayerRole;
	SacrificePlayerState->UserState = NewState;
	SacrificePlayerState->OnRep_UserState();
}

void ASacrificePlayerController::ClientRPC_OnSkillCheck_Implementation(AActor* Obj, const float Min, const float Max,
                                                                       const float GreatRange)
{
	if (Hud)
	{
		SkillCheckableObject = Obj;
		NET_LOG(LogTemp, Warning, TEXT("ClientRPC_OnSkillCheck_Implementation"));
		Hud->OnSkillCheck(Min, Max, GreatRange);
	}
}

void ASacrificePlayerController::SkillCheckFinish(const ESkillCheckResult Result)
{
	ServerRPC_SkillCheckFinish(SkillCheckableObject, Result);
}

void ASacrificePlayerController::ServerRPC_SkillCheckFinish_Implementation(AActor* Obj, const ESkillCheckResult Result)
{
	NET_LOG(LogTemp, Warning, TEXT("ServerRPC_SkillCheckFinish_Implementation 1"));
	if (auto* Camper = Cast<ACamper>(GetPawn()))
	{
		NET_LOG(LogTemp, Warning, TEXT("ServerRPC_SkillCheckFinish_Implementation 2"));
		if (auto* Generator = Cast<AGenerator>(Obj))
		{
			NET_LOG(LogTemp, Warning, TEXT("ServerRPC_SkillCheckFinish_Implementation 3"));
			Generator->SkillCheckFinish(Camper, Result);
		}
	}
}