// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoorAnim.h"

#include "ExitDoor.h"

void UExitDoorAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	ExitDoor = Cast<AExitDoor>(GetOwningActor());
	if (ExitDoor)
	{
		ExitDoor->OnOpenExitDoor.AddDynamic(this, &UExitDoorAnim::OnOpenExitDoor);
	}
}

void UExitDoorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (ExitDoor)
	{
		bIsActivating = ExitDoor->bIsActivating;
	}
}

void UExitDoorAnim::OnOpenExitDoor()
{
	bIsDoorOpened = true;
}
