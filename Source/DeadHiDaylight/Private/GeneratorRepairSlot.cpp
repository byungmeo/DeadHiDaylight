// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorRepairSlot.h"


UGeneratorRepairSlot::UGeneratorRepairSlot()
{
	PrimaryComponentTick.bCanEverTick = true;
	OnComponentBeginOverlap.AddDynamic(this, &UGeneratorRepairSlot::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UGeneratorRepairSlot::OnEndOverlap);
}

void UGeneratorRepairSlot::BeginPlay()
{
	Super::BeginPlay();
	Generator = Cast<AGenerator>(GetOwner());
}

void UGeneratorRepairSlot::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGeneratorRepairSlot::Init(USkeletalMeshComponent* GeneratorMesh, const FName BoneName)
{
	SetupAttachment(GeneratorMesh, BoneName);
	BindBoneName = BoneName;
	SetBoxExtent(FVector(32, 50, 40));
	SetActive(true);
}

void UGeneratorRepairSlot::AttachCamper(ACamper* Camper)
{
	if (nullptr != AttachedCamper)
	{
		return;
	}
	Camper->AttachToComponent(Generator->Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, BindBoneName);
	Camper->SetActorRotation(GetComponentRotation());
	AttachedCamper = Camper;
	SetActive(false);
}

void UGeneratorRepairSlot::DetachCamper()
{
	if (nullptr == AttachedCamper)
	{
		return;
	}
	AttachedCamper->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	AttachedCamper = nullptr;
	SetActive(true);
}

void UGeneratorRepairSlot::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		Camper->BeginGeneratorOverlap(this);
	}
}

void UGeneratorRepairSlot::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		Camper->EndGeneratorOverlap(this);
	}
}
