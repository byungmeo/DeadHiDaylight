// Fill out your copyright notice in the Description page of Project Settings.


#include "PalletAnim.h"

#include "Pallet.h"

void UPalletAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Pallet = Cast<APallet>(GetOwningActor());
	if (Pallet)
	{
		Pallet->OnFallGround.AddDynamic(this, &UPalletAnim::OnFallGround);
	}
}