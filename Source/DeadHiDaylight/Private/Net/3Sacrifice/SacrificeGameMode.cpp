// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeGameMode.h"

#include "Camper.h"
#include "DHDGameInstance.h"
#include "DHDPlayerState.h"
#include "Generator.h"
#include "MeatHook.h"
#include "Observer.h"
#include "SacrificeGameState.h"
#include "SacrificePlayerController.h"
#include "DeadHiDaylight/Canival.h"
#include "Kismet/GameplayStatics.h"

ASacrificeGameMode::ASacrificeGameMode()
{
	GameStateClass = ASacrificeGameState::StaticClass();
	PlayerControllerClass = ASacrificePlayerController::StaticClass();
	PlayerStateClass = ADHDPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
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

void ASacrificeGameMode::RequestCreatePawn(ASacrificePlayerController* Controller, const EPlayerRole PlayerRole)
{
	switch (PlayerRole)
	{
	case EPlayerRole::EPR_Observer:
		{
			UE_LOG(LogTemp, Warning, TEXT("SacrificeGameMode::RequestCreatePawn Observer"));
			if (AObserver* Observer = GetWorld()->SpawnActor<AObserver>(AObserver::StaticClass()))
			{
				Controller->Possess(Observer);
			}
			break;
		}
	case EPlayerRole::EPR_Slasher:
		{
			UE_LOG(LogTemp, Warning, TEXT("SacrificeGameMode::RequestCreatePawn Slasher"));
			if (ACanival* Canival = GetWorld()->SpawnActor<ACanival>(SlasherFactory))
			{
				Canival->SetActorLocation(FVector(1410, 3440, 250));
				Controller->Possess(Canival);
				Slasher = Canival;
			}
			break;
		}
	case EPlayerRole::EPR_Camper:
		{
			UE_LOG(LogTemp, Warning, TEXT("SacrificeGameMode::RequestCreatePawn Camper"));
			if (ACamper* Camper = GetWorld()->SpawnActor<ACamper>(CamperFactory))
			{
				Camper->SetActorLocation(FVector(-1620, 1580, 250));
				Controller->Possess(Camper);
				Campers.Add(Camper);
			}
			break;
		}
	case EPlayerRole::EPR_None:
		{
			UE_LOG(LogTemp, Warning, TEXT("SacrificeGameMode::RequestCreatePawn None"));
			break;
		}
	}
}
