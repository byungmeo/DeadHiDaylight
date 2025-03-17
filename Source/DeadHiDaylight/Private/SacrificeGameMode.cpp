// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeGameMode.h"

#include "SacrificeGameState.h"

ASacrificeGameMode::ASacrificeGameMode()
{
	GameStateClass = ASacrificeGameState::StaticClass();
	PlayerStateClass = nullptr;
	DefaultPawnClass = nullptr;
}
