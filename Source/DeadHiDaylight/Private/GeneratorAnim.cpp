// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorAnim.h"

#include "Generator.h"

void UGeneratorAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Generator = Cast<AGenerator>(GetOwningActor());
	if (Generator)
	{
		Generator->OnExplosion.AddDynamic(this, &UGeneratorAnim::OnExplosion);
	}
}

void UGeneratorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Generator)
	{
		bPowerOn = Generator->bPowerOn;
		bIsExplosion = Generator->bIsExplosion;
		PowerGauge = Generator->PowerGauge;
	}
}
