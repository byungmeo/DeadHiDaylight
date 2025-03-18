// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificePlayerController.h"

#include "SacrificeGameMode.h"
#include "DeadHiDaylight/Canival.h"
#include "Camper.h"
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
		if (GameMode->Slasher)
		{
			Players.Add(GameMode->Slasher);
		}
		for (auto* Camper : GameMode->Campers)
		{
			Players.Add(Camper);
		}
	}
}

void ASacrificePlayerController::PrevPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("ASacrificePlayerController::PrevPlayer"));
	if (bIsFreeMode || Players.Num() == 0)
	{
		return;
	}
	
	if (--SpectatorIndex < 0)
	{
		SpectatorIndex = Players.Num() - 1;
	}

	SetViewTargetWithBlend(Players[SpectatorIndex], 0.5f);
}

void ASacrificePlayerController::NextPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("ASacrificePlayerController::NextPlayer"));
	if (bIsFreeMode)
	{
		return;
	}

	if (++SpectatorIndex >= Players.Num())
	{
		SpectatorIndex = 0;
	}
	
	SetViewTargetWithBlend(Players[SpectatorIndex], 0.5f);
}

void ASacrificePlayerController::ToggleCamera()
{
	UE_LOG(LogTemp, Warning, TEXT("ASacrificePlayerController::ToggleCamera"));
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
		if (Players[SpectatorIndex])
		{
			SetViewTargetWithBlend(Players[SpectatorIndex], 0.5f);
		}
	}
}
