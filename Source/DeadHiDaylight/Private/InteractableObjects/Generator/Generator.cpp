// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "Camper.h"
#include "InteractionPoint.h"


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
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KBD/Generator/ABP_Generator.ABP_Generator_C'"));
	if (AnimClass.Succeeded())
	{
		Mesh->SetAnimInstanceClass(AnimClass.Class);
	}

	if (Mesh)
	{
		PointLeft = CreateDefaultSubobject<UInteractionPoint>(TEXT("PointLeft"));
		PointLeft->SetupAttachment(Mesh, TEXT("joint_CamperAttach01"));
		PointLeft->SetRelativeRotation(FRotator(0, 180, 90));
		PointLeft->OnInteraction.AddDynamic(this, &AGenerator::OnInteraction);
		PointLeft->OnStopInteraction.AddDynamic(this, &AGenerator::OnStopInteraction);
		
		PointRight = CreateDefaultSubobject<UInteractionPoint>(TEXT("PointRight"));
		PointRight->SetupAttachment(Mesh, TEXT("joint_CamperAttach02"));
		PointRight->SetRelativeRotation(FRotator(0, 0, -90));
		PointRight->OnInteraction.AddDynamic(this, &AGenerator::OnInteraction);
		PointRight->OnStopInteraction.AddDynamic(this, &AGenerator::OnStopInteraction);
		
		PointFront = CreateDefaultSubobject<UInteractionPoint>(TEXT("PointFront"));
		PointFront->SetupAttachment(Mesh, TEXT("joint_CamperAttach03"));
		PointFront->SetRelativeRotation(FRotator(-90, 0, -90));
		PointFront->OnInteraction.AddDynamic(this, &AGenerator::OnInteraction);
		PointFront->OnStopInteraction.AddDynamic(this, &AGenerator::OnStopInteraction);
		
		PointBack = CreateDefaultSubobject<UInteractionPoint>(TEXT("PointBack"));
		PointBack->SetupAttachment(Mesh, TEXT("joint_CamperAttach04"));
		PointBack->SetRelativeRotation(FRotator(90, 0, -90));
		PointBack->OnInteraction.AddDynamic(this, &AGenerator::OnInteraction);
		PointBack->OnStopInteraction.AddDynamic(this, &AGenerator::OnStopInteraction);
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
	RepairValue = (BaseRepairValue - (BaseRepairValue * ReductionRepairEfficiency * (RepairingCount - 1))) * RepairingCount;
	
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
	DetachAll();
	DestroyPointsAll();
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

void AGenerator::DetachAll() const
{
	PointLeft->DetachActor();
	PointRight->DetachActor();
	PointFront->DetachActor();
	PointBack->DetachActor();
}

void AGenerator::DestroyPointsAll() const
{
	PointLeft->DestroyComponent();
	PointRight->DestroyComponent();
	PointFront->DestroyComponent();
	PointBack->DestroyComponent();
}

void AGenerator::OnInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AGenerator::OnInteraction Survivor"));
		Point->AttachActor(OtherActor);
		RepairingCount++;
		Camper->StartRepair();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGenerator::OnInteraction Slasher"));
		// NotifyStartBreak();
	}
}

void AGenerator::OnStopInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AGenerator::OnInteraction Survivor"));
		Point->DetachActor();
		RepairingCount--;
		Camper->EndRepair();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGenerator::OnInteraction Slasher"));
		// NotifyEndBreak();
	}
}
