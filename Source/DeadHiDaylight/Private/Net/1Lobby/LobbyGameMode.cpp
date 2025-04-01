// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "DHDGameInstance.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
	DefaultPawnClass = nullptr;
}

void ALobbyGameMode::CreateRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode::CreateRoom"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Common/Maps/CampfireMap"), true, "listen");
}

void ALobbyGameMode::JoinRoom(const FString& ServerAddress, const FText& Nickname)
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode::JoinRoom"));
	auto* GameInstance = Cast<UDHDGameInstance>(GetGameInstance());
	GameInstance->Nickname = Nickname;
	UGameplayStatics::OpenLevel(GetWorld(), *ServerAddress);
}