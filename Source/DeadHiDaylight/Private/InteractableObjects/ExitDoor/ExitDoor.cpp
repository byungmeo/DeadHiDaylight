// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor.h"

#include "Camper.h"
#include "InteractionPoint.h"
#include "DeadHiDaylight/DeadHiDaylight.h"


// Sets default values
AExitDoor::AExitDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KBD/ExitDoor/ExitDoor.ExitDoor'"));
	if (MeshObj.Succeeded())
	{
		Mesh->SetSkeletalMeshAsset(MeshObj.Object);
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KBD/ExitDoor/ABP_ExitDoor.ABP_ExitDoor_C'"));
	if (AnimClass.Succeeded())
	{
		Mesh->SetAnimInstanceClass(AnimClass.Class);
	}

	if (Mesh)
	{
		CamperPoint = CreateDefaultSubobject<UInteractionPoint>(TEXT("CamperPoint"));
		CamperPoint->SetupAttachment(Mesh, TEXT("joint_AttachCamper"));
		CamperPoint->SetRelativeRotation(FRotator(0, 0, -90));
		CamperPoint->OnInteraction.AddDynamic(this, &AExitDoor::OnInteraction);
		CamperPoint->OnStopInteraction.AddDynamic(this, &AExitDoor::OnStopInteraction);
	}
}

// Called every frame
void AExitDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (true == bDoorOpened || false == bIsDoorOpening)
	{
		return;
	}
	
	PowerGauge += BasePowerValue * DeltaTime;
	if (PowerGauge >= 1)
	{
		OpenExitDoor();
	}
}

void AExitDoor::OpenExitDoor()
{
	NET_LOG(LogTemp, Warning, TEXT("AExitDoor::OpenExitDoor"));
	bDoorOpened = true;
	OnStopInteraction(CamperPoint, CamperPoint->AttachedActor);
	CamperPoint->DestroyComponent();
	OnOpenExitDoor.Broadcast();
	SetActorTickEnabled(false);
}

void AExitDoor::OnInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		NET_LOG(LogTemp, Warning, TEXT("AExitDoor::OnInteraction Survivor"));
		bIsDoorOpening = true;
		float OrgZ = OtherActor->GetActorLocation().Z;
		Point->AttachActor(OtherActor);
		FVector NewLocation = OtherActor->GetActorLocation();
		NewLocation.Z = OrgZ;
		OtherActor->SetActorLocation(NewLocation);
		// Camper->StartOpenExitDoor();
	}
}

void AExitDoor::OnStopInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		NET_LOG(LogTemp, Warning, TEXT("AExitDoor::OnInteraction Survivor"));
		bIsDoorOpening = false;
		Point->DetachActor();
		// Camper->EndOpenExitDoor();
	}
}

