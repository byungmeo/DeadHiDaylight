// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

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

void ALobbyGameMode::JoinRoom(const FString& ServerAddress)
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode::JoinRoom"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("127.0.0.1"));
}