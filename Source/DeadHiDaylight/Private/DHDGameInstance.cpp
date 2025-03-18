// Fill out your copyright notice in the Description page of Project Settings.


#include "DHDGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UDHDGameInstance::CreateRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("UDHDGameInstance::CreateRoom"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Common/Maps/CampfireMap"), true, "listen");
}

void UDHDGameInstance::JoinRoom(const FString& ServerAddress)
{
	UE_LOG(LogTemp, Warning, TEXT("UDHDGameInstance::JoinRoom"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("127.0.0.1"));
}