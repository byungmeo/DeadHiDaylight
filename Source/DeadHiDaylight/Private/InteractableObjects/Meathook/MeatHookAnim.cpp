// Fill out your copyright notice in the Description page of Project Settings.


#include "MeatHookAnim.h"

#include "Meathook.h"

void UMeatHookAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	MeatHook = Cast<AMeatHook>(GetOwningActor());
}
