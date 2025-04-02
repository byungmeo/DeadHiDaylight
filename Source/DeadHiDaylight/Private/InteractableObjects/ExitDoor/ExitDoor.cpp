// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor.h"

#include "Player/Camper.h"
#include "InteractionPoint.h"
#include "SacrificeGameState.h"
#include "SacrificePlayerState.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AExitDoor::AExitDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KBD/ExitDoor/ExitDoor.ExitDoor'"));
	if (MeshObj.Succeeded())
	{
		Mesh->SetSkeletalMeshAsset(MeshObj.Object);
		Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KBD/ExitDoor/ABP_ExitDoor.ABP_ExitDoor_C'"));
	if (AnimClass.Succeeded())
	{
		Mesh->SetAnimInstanceClass(AnimClass.Class);
	}

	CamperPoint = CreateDefaultSubobject<UInteractionPoint>(TEXT("CamperPoint"));
	CamperPoint->SetupAttachment(Mesh, TEXT("joint_AttachCamper"));
	CamperPoint->SetRelativeRotation(FRotator(0, 0, -90));
	CamperPoint->OnInteraction.AddDynamic(this, &AExitDoor::OnInteraction);
	CamperPoint->OnStopInteraction.AddDynamic(this, &AExitDoor::OnStopInteraction);
	CamperPoint->InteractionMode = EInteractionMode::EIM_CamperOnly;

	ExitArea = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitArea"));
	ExitArea->SetupAttachment(Mesh);
	ExitArea->SetRelativeLocation(FVector(500, 0, 0));
	ExitArea->SetBoxExtent(FVector(200, 1000, 1000));
}

void AExitDoor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && ExitArea)
	{
		ExitArea->OnComponentBeginOverlap.AddDynamic(this, &AExitDoor::ServerOnly_OnExitAreaBeginOverlap);
	}
}

// Called every frame
void AExitDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (false == HasAuthority())
	{
		return;
	}

	if (true == bIsDoorOpened || false == bIsActivating)
	{
		return;
	}
	
	PowerGauge += BasePowerValue * DeltaTime;
	if (PowerGauge >= 1)
	{
		OpenExitDoor();
	}
}

void AExitDoor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExitDoor, bIsDoorOpened);
	DOREPLIFETIME(AExitDoor, bIsActivating);
	DOREPLIFETIME(AExitDoor, PowerGauge);
}

void AExitDoor::OpenExitDoor()
{
	NET_LOG(LogTemp, Warning, TEXT("AExitDoor::OpenExitDoor"));
	bIsDoorOpened = true;
	OnStopInteraction(CamperPoint, CamperPoint->AttachedActor);
	CamperPoint->DestroyComponent();
	SetActorTickEnabled(false);
}

void AExitDoor::StartActivatingPIE()
{
	bIsActivating = true;
}

void AExitDoor::StopActivatingPIE()
{
	bIsActivating = false;
}

void AExitDoor::OnInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		NET_LOG(LogTemp, Warning, TEXT("AExitDoor::OnInteraction Survivor"));
		bIsActivating = true;
		Point->AttachActor(OtherActor, 0, true);
		Camper->StartUnLock(); // 정솔 추가 부분
	}
}

void AExitDoor::OnStopInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		NET_LOG(LogTemp, Warning, TEXT("AExitDoor::OnInteraction Survivor"));
		bIsActivating = false;
		Point->DetachActor();
		Camper->EndUnLock(); // 정솔 추가 부분
	}
}

void AExitDoor::ServerOnly_OnExitAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* Camper = Cast<ACamper>(OtherActor))
	{
		if (Camper->GetMesh()->IsVisible() == false)
		{
			return;
		}
		
		NET_LOG(LogTemp, Warning, TEXT("AExitDoor::OnExitAreaBeginOverlap : 생존자 탈출 성공"));
		auto* CamperState = Cast<ASacrificePlayerState>(Camper->GetPlayerState());
		if (CamperState)
		{
			// TODO: 탈출 처리
			auto* GameState = Cast<ASacrificeGameState>(GetWorld()->GetGameState());
			if (GameState)
			{
				GameState->ServerOnly_OnCamperExitOrDie();
			}
		}
		
		// Camper->UnPossessed();
		Camper->GetMesh()->SetVisibility(false);
		Camper->SetActorEnableCollision(false);
		Camper->CrawlPoint->DestroyComponent();
		Camper->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		Camper->GetCharacterMovement()->StopMovementImmediately();
	}
}
