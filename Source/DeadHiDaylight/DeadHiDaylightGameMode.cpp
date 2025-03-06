// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadHiDaylightGameMode.h"
#include "DeadHiDaylightCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADeadHiDaylightGameMode::ADeadHiDaylightGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
