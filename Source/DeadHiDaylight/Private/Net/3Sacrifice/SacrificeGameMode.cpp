// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeGameMode.h"

#include "CamperSpawnPoint.h"
#include "Player/Camper.h"
#include "DHDGameInstance.h"
#include "Generator.h"
#include "MeatHook.h"
#include "Observer.h"
#include "SacrificeGameState.h"
#include "SacrificePlayerController.h"
#include "SacrificePlayerState.h"
#include "Canival.h"
#include "SlasherSpawnPoint.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Kismet/GameplayStatics.h"

ASacrificeGameMode::ASacrificeGameMode()
{
	GameStateClass = ASacrificeGameState::StaticClass();
	PlayerControllerClass = ASacrificePlayerController::StaticClass();
	PlayerStateClass = ASacrificePlayerState::StaticClass();
}

void ASacrificeGameMode::InitGameState()
{
	Super::InitGameState();
	SacrificeGameState = Cast<ASacrificeGameState>(GameState);
}

void ASacrificeGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenerator::StaticClass(), OutActors);
	for (auto* Actor : OutActors)
	{
		AGenerator* Generator = Cast<AGenerator>(Actor);
		Generators.Add(Generator);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMeatHook::StaticClass(), OutActors);
	for (auto* Actor : OutActors)
	{
		AMeatHook* MeatHook = Cast<AMeatHook>(Actor);
		MeatHooks.Add(MeatHook);
	}
}

void ASacrificeGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ASacrificeGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (ASacrificePlayerState* PlayerState = Exiting->GetPlayerState<ASacrificePlayerState>())
	{
		
	}
}

void ASacrificeGameMode::RequestCreatePawn(ASacrificePlayerController* Controller, const EPlayerRole PlayerRole)
{
	if (Controller->IsLocalController())
	{
		NET_LOG(LogTemp, Warning, TEXT("SacrificeGameMode::RequestCreatePawn Observer"));
		if (AObserver* Observer = GetWorld()->SpawnActor<AObserver>(AObserver::StaticClass()))
		{
			Controller->Possess(Observer);
		}
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	switch (PlayerRole)
	{
	case EPlayerRole::EPR_Slasher:
		{
			NET_LOG(LogTemp, Warning, TEXT("SacrificeGameMode::RequestCreatePawn Slasher"));
			auto* SlasherSpawnPoint = Cast<ASlasherSpawnPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), ASlasherSpawnPoint::StaticClass()));
			if (SlasherSpawnPoint)
			{
				if (ACanival* Canival = GetWorld()->SpawnActor<ACanival>(SlasherFactory, SlasherSpawnPoint->GetActorLocation(), SlasherSpawnPoint->GetActorRotation(), SpawnParams))
				{
					Controller->Possess(Canival);
					Slasher = Canival;
				}
			}
			else
			{
				
			}
			break;
		}
	case EPlayerRole::EPR_Camper:
		{
			NET_LOG(LogTemp, Warning, TEXT("SacrificeGameMode::RequestCreatePawn Camper"));
			TArray<AActor*> OutActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACamperSpawnPoint::StaticClass(), OutActors);
			if (OutActors.Num() > 0)
			{
				auto* CamperSpawnPoint = Cast<ACamperSpawnPoint>(OutActors[FMath::RandRange(0, OutActors.Num() - 1)]);
				if (ACamper* Camper = GetWorld()->SpawnActor<ACamper>(CamperFactory, CamperSpawnPoint->GetActorLocation(), CamperSpawnPoint->GetActorRotation(), SpawnParams))
				{
					Controller->Possess(Camper);
					Campers.Add(Camper);
				}
			}
			break;
		}
	case EPlayerRole::EPR_None:
		{
			NET_LOG(LogTemp, Warning, TEXT("SacrificeGameMode::RequestCreatePawn None"));
			break;
		}
	default:
		break;
	}
}