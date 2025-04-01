// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "Player/Camper.h"
#include "Canival.h"
#include "InteractionPoint.h"
#include "SacrificePlayerController.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AGenerator::AGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KBD/Generator/Generator.Generator'"));
	if (MeshObj.Succeeded())
	{
		Mesh->SetSkeletalMeshAsset(MeshObj.Object);
		Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
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
		PointLeft->InteractionMode = EInteractionMode::EIM_Both;
		PointLeft->bSkillCheckEnable = true;
		PointLeft->OnSkillCheck.AddDynamic(this, &AGenerator::OnSkillCheck);
		
		PointRight = CreateDefaultSubobject<UInteractionPoint>(TEXT("PointRight"));
		PointRight->SetupAttachment(Mesh, TEXT("joint_CamperAttach02"));
		PointRight->SetRelativeRotation(FRotator(0, 0, -90));
		PointRight->OnInteraction.AddDynamic(this, &AGenerator::OnInteraction);
		PointRight->OnStopInteraction.AddDynamic(this, &AGenerator::OnStopInteraction);
		PointRight->InteractionMode = EInteractionMode::EIM_Both;
		PointRight->bSkillCheckEnable = true;
		PointRight->OnSkillCheck.AddDynamic(this, &AGenerator::OnSkillCheck);
		
		PointFront = CreateDefaultSubobject<UInteractionPoint>(TEXT("PointFront"));
		PointFront->SetupAttachment(Mesh, TEXT("joint_CamperAttach03"));
		PointFront->SetRelativeRotation(FRotator(-90, 0, -90));
		PointFront->OnInteraction.AddDynamic(this, &AGenerator::OnInteraction);
		PointFront->OnStopInteraction.AddDynamic(this, &AGenerator::OnStopInteraction);
		PointFront->InteractionMode = EInteractionMode::EIM_Both;
		PointFront->bSkillCheckEnable = true;
		PointFront->OnSkillCheck.AddDynamic(this, &AGenerator::OnSkillCheck);
		
		PointBack = CreateDefaultSubobject<UInteractionPoint>(TEXT("PointBack"));
		PointBack->SetupAttachment(Mesh, TEXT("joint_CamperAttach04"));
		PointBack->SetRelativeRotation(FRotator(90, 0, -90));
		PointBack->OnInteraction.AddDynamic(this, &AGenerator::OnInteraction);
		PointBack->OnStopInteraction.AddDynamic(this, &AGenerator::OnStopInteraction);
		PointBack->InteractionMode = EInteractionMode::EIM_Both;
		PointBack->bSkillCheckEnable = true;
		PointBack->OnSkillCheck.AddDynamic(this, &AGenerator::OnSkillCheck);
	}
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (false == HasAuthority())
	{
		return;
	}
	
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

void AGenerator::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGenerator, bPowerOn);
	DOREPLIFETIME(AGenerator, PowerGauge);
	DOREPLIFETIME(AGenerator, bIsExplosion);
}


void AGenerator::PowerOn()
{
	bPowerOn = true;
	DetachAll();
	DestroyPointsAll();
	MulticastRPC_PowerOn();
	SetActorTickEnabled(false);
}

void AGenerator::MulticastRPC_PowerOn_Implementation()
{
	OnPowerOn.Broadcast();
}

void AGenerator::OnSkillCheck(AActor* TargetActor)
{
	if (bPowerOn)
	{
		return;
	}
	
	if (ACamper* Camper = Cast<ACamper>(TargetActor))
	{
		NET_LOG(LogTemp, Warning, TEXT("AGenerator::OnSkillCheck"));
		auto* PlayerController = Cast<ASacrificePlayerController>(Camper->GetController());
		if (PlayerController)
		{
			const float Min = FMath::RandRange(0.38f, 0.8f);
			const float Max = Min + 0.15f;
			const float GreatRange = 0.05f;
			PlayerController->ClientRPC_OnSkillCheck(this, Min, Max, GreatRange);
		}
		// Camper->StartSkillCheck();
	}
}

void AGenerator::SkillCheckFinish(class ACamper* Camper, const ESkillCheckResult Result)
{
	if (bPowerOn)
	{
		return;
	}
	
	switch (Result)
    	{
    	case ESkillCheckResult::ESCR_Fail:
    		SkillCheckFail(Camper);
    		break;
    	case ESkillCheckResult::ESCR_Success:
    		SkillCheckSuccess(false);
    		break;
    	case ESkillCheckResult::ESCR_GreatSuccess:
    		SkillCheckSuccess(true);
    		break;
    	}
}

void AGenerator::SkillCheckSuccess(const bool bGreatSuccess)
{
	NET_LOG(LogTemp, Warning, TEXT("AGenerator::SkillCheckSuccess %hs"), (bGreatSuccess ? "GREAT!!!" : "SOSO..."));
	
	if (true == bGreatSuccess)
	{
		PowerGauge += GreateSuccessBonus;
	}
}

void AGenerator::SkillCheckFail(ACamper* Camper)
{
	NET_LOG(LogTemp, Warning, TEXT("AGenerator::SkillCheckFail"));
	
	bIsExplosion = true;
	PowerGauge = FMath::Clamp(PowerGauge - ImmediateExplosionValue, 0, 1);
	RemainExplosionTime = ExplosionDuration;

	MulticastRPC_SkillCheckFail(Camper);
}

void AGenerator::MulticastRPC_SkillCheckFail_Implementation(ACamper* Camper)
{
	OnExplosion.Broadcast();
	if (Camper)
    {
    	// Camper->GeneratorSkillCheckFail();
    }
}

void AGenerator::TestExplosion()
{
	SkillCheckFail(nullptr);
}

void AGenerator::Break(UInteractionPoint* Point)
{
	if (bPowerOn)
	{
		return;
	}

	Point->DetachActor();
	bIsBreak = true;
	PowerGauge -= FMath::Clamp(ImmediateBreakValue, 0, 1);
	RemainBreakShield = InitBreakShield;
	OnBreak.Broadcast();
}

void AGenerator::TestBreak()
{
	Break(nullptr);
}

void AGenerator::DetachAll() const
{
	PointLeft->StopInteraction(PointLeft->AttachedActor);
	PointRight->StopInteraction(PointRight->AttachedActor);
	PointFront->StopInteraction(PointFront->AttachedActor);
	PointBack->StopInteraction(PointBack->AttachedActor);
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
		Point->AttachActor(OtherActor, 0, true);
		RepairingCount++;
		Camper->MultiCastRPC_StartRepair();
	}
	else if (ACanival* Slasher = Cast<ACanival>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AGenerator::OnInteraction Slasher"));
		Point->AttachActor(Slasher, -100, true);
		Slasher->KickGenerator(Point);
	}
}

void AGenerator::OnStopInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
	if (nullptr == OtherActor)
	{
		return;
	}
	
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AGenerator::OnStopInteraction Survivor"));
		Point->DetachActor();
		RepairingCount--;
		Camper->MultiCastRPC_EndRepair();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGenerator::OnStopInteraction Slasher"));
		// NotifyEndBreak();
	}
}
