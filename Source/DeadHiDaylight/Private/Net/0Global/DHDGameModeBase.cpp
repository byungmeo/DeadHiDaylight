// Fill out your copyright notice in the Description page of Project Settings.


#include "DHDGameModeBase.h"

#include "DHDGameInstance.h"
#include "DHDPlayerState.h"
#include "DeadHiDaylight/DeadHiDaylight.h"

ADHDGameModeBase::ADHDGameModeBase()
{
	PlayerStateClass = ADHDPlayerState::StaticClass();
	GameSessionClass = nullptr;
	DefaultPawnClass = nullptr;
	SpectatorClass = nullptr;
	ReplaySpectatorPlayerControllerClass = nullptr;
}

void ADHDGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	NET_LOG(LogTemp, Warning, TEXT("ADHDGameModeBase::InitGame"));
	ServerGameInstance = Cast<UDHDGameInstance>(GetGameInstance());
}

void ADHDGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	NET_LOG(LogTemp, Warning, TEXT("ADHDGameModeBase::BeginPlay"));
}
