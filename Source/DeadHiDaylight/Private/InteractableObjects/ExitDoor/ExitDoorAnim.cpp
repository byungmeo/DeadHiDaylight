// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoorAnim.h"

#include "ExitDoor.h"

void UExitDoorAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	ExitDoor = Cast<AExitDoor>(GetOwningActor());
}
