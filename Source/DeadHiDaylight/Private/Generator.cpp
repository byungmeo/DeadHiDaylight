// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadHiDaylight/Public/Generator.h"

#include "Camper.h"
#include "GeneratorAnim.h"
#include "GeneratorRepairSlot.h"
#include "Components/BoxComponent.h"


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

	if (Mesh)
	{
		SlotLeft = CreateDefaultSubobject<UGeneratorRepairSlot>(TEXT("SlotLeft"));
		SlotLeft->Init(Mesh, TEXT("joint_CamperAttach01"));
		SlotLeft->SetRelativeRotation(FRotator(0, 180, 90));
	
		SlotRight = CreateDefaultSubobject<UGeneratorRepairSlot>(TEXT("SlotRight"));
		SlotRight->Init(Mesh, TEXT("joint_CamperAttach02"));
		SlotRight->SetRelativeRotation(FRotator(0, 0, -90));
	
		SlotFront = CreateDefaultSubobject<UGeneratorRepairSlot>(TEXT("SlotFront"));
		SlotFront->Init(Mesh, TEXT("joint_CamperAttach03"));
		SlotFront->SetRelativeRotation(FRotator(-90, 0, -90));
	
		SlotBack = CreateDefaultSubobject<UGeneratorRepairSlot>(TEXT("SlotBack"));
		SlotBack->Init(Mesh, TEXT("joint_CamperAttach04"));
		SlotBack->SetRelativeRotation(FRotator(90, 0, -90));
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
	
	float RepairValue = 0;
	if (RepairingCount > 0)
	{
		RepairValue = BaseRepairValue - (BaseRepairValue * ReductionRepairEfficiency * (RepairingCount - 1));
	}
	
	if (bIsBreak)
	{
		PowerGauge -= ProgressiveBreakValue * DeltaTime;
		
		RemainBreakShield -= RepairValue * DeltaTime;
		if (RemainBreakShield <= 0)
		{
			bIsBreak = false;
		}
		return;
	}
	
	PowerGauge += RepairValue * DeltaTime;
	if (PowerGauge >= 1)
	{
		PowerOn();
	}
}

void AGenerator::PowerOn()
{
	bPowerOn = true;
	
	SlotLeft->DetachCamper();
	SlotRight->DetachCamper();
	SlotFront->DetachCamper();
	SlotBack->DetachCamper();
	
	SlotLeft->DestroyComponent();
	SlotRight->DestroyComponent();
	SlotFront->DestroyComponent();
	SlotBack->DestroyComponent();
	
	OnPowerOn.Broadcast();
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

void AGenerator::Break()
{
	bIsBreak = true;
	PowerGauge -= FMath::Clamp(ImmediateBreakValue, 0, 1);
	RemainBreakShield = InitBreakShield;
	OnBreak.Broadcast();
}

void AGenerator::TestBreak()
{
	Break();
}

void AGenerator::Interact(ACharacter* Character)
{
	const UBoxComponent* Box = FindOverlapBox(Character);
	if (nullptr == Box)
	{
		return;
	}

	if (ACamper* Camper = Cast<ACamper>(Character))
	{
		NotifyStartRepair(Camper);
	}
	else
	{
		NotifyStartBreak();
	}
}

void AGenerator::NotifyStartRepair(ACamper* Camper)
{
	UGeneratorRepairSlot* OverlappedBox = FindOverlapBox(Camper);
	if (nullptr == OverlappedBox)
	{
		return;
	}
	
	OverlappedBox->AttachCamper(Camper);
	RepairingCount++;
	
	// TODO: 생존자에게 발전기 수리를 시작하라고 알린다.
}

void AGenerator::NotifyEndRepair(const ACamper* Camper)
{
	UGeneratorRepairSlot* Box = FindBoxByCamper(Camper);
	if (nullptr == Box)
	{
		return;
	}
	Box->DetachCamper();
	RepairingCount--;

	// TODO: 생존자에게 발전기 수리를 멈추라고 알린다.
}

void AGenerator::NotifyStartBreak()
{
	// TODO: 살인마에게 발전기 부수기를 시작해도 된다고 알린다.
}

UGeneratorRepairSlot* AGenerator::FindBoxByCamper(const ACamper* Camper)
{
	if (SlotLeft && SlotLeft->AttachedCamper == Camper)
    {
    	return SlotLeft;
    }
    if (SlotRight && SlotRight->AttachedCamper == Camper)
    {
    	return SlotLeft;
    }
    if (SlotFront && SlotFront->AttachedCamper == Camper)
    {
    	return SlotFront;
    }
    if (SlotBack && SlotBack->AttachedCamper == Camper)
    {
    	return SlotBack;
    }
	return nullptr;
}

UGeneratorRepairSlot* AGenerator::FindOverlapBox(const ACharacter* Character)
{
	if (SlotLeft && SlotLeft->IsActive() && SlotLeft->IsOverlappingActor(Character))
	{
		return SlotLeft;
	}
	if (SlotRight && SlotRight->IsActive() && SlotRight->IsOverlappingActor(Character))
	{
		return SlotLeft;
	}
	if (SlotFront && SlotFront->IsActive() && SlotFront->IsOverlappingActor(Character))
	{
		return SlotFront;
	}
	if (SlotBack && SlotBack->IsActive() && SlotBack->IsOverlappingActor(Character))
	{
		return SlotBack;
	}
	return nullptr;
}