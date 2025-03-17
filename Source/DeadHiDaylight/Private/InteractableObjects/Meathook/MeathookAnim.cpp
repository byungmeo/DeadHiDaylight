// Fill out your copyright notice in the Description page of Project Settings.


#include "MeathookAnim.h"

#include "Meathook.h"

void UMeathookAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Meathook = Cast<AMeathook>(GetOwningActor());
}
