// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadHiDaylight/Public/Generator.h"

#include "GeneratorAnim.h"


// Sets default values
AGenerator::AGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KBD/Generator/Generator.Generator'"));
	if (MeshObj.Succeeded())
	{
		Mesh->SetSkeletalMeshAsset(MeshObj.Object);
	}
	static ConstructorHelpers::FClassFinder<UGeneratorAnim> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KBD/Generator/ABP_Generator.ABP_Generator_C'"));
	if (AnimClass.Succeeded())
	{
		Mesh->SetAnimInstanceClass(AnimClass.Class);
	}
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPowerOn)
	{
		return;
	}

	if (bIsExplosion)
	{
		RemainExplosionTime -= DeltaTime;
		if (RemainExplosionTime <= 0)
		{
			bIsExplosion = false;
		}
		return;
	}
	
	PowerGauge += BaseRepairValue * DeltaTime;
	if (PowerGauge >= 1)
	{
		bPowerOn = true;
	}
}

void AGenerator::SkillCheckSuccess(const bool bGreateSuccess)
{
	if (true == bGreateSuccess)
	{
		PowerGauge += GreateSuccessBonus;
	}
}

void AGenerator::SkillCheckFail()
{
	bIsExplosion = true;
	PowerGauge -= FMath::Clamp(ImmediateExplosionValue, 0, 1);
	RemainExplosionTime = ExplosionDuration;
	OnExplosion.Broadcast();
}

void AGenerator::TestExplosion()
{
	SkillCheckFail();
}