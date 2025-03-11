// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorAnim.h"

#include "Generator.h"

void UGeneratorAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Generator = Cast<AGenerator>(GetOwningActor());
}

void UGeneratorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Generator)
	{
		PowerGauge = Generator->PowerGauge;
	}
}
