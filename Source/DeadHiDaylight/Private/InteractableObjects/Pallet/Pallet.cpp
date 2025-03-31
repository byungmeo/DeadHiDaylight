// Fill out your copyright notice in the Description page of Project Settings.


#include "Pallet.h"

#include "Canival.h"
#include "InteractionPoint.h"
#include "Player/Camper.h"


// Sets default values
APallet::APallet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KBD/Pallet/Pallet.Pallet'"));
	if (MeshObj.Succeeded())
	{
		Mesh->SetSkeletalMeshAsset(MeshObj.Object);
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KBD/Pallet/ABP_Pallet.ABP_Pallet_C'"));
	if (AnimClass.Succeeded())
	{
		Mesh->SetAnimInstanceClass(AnimClass.Class);
	}

	BrokenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrokenMesh"));
	static const ConstructorHelpers::FObjectFinder<UStaticMesh> BrokenMeshObj(TEXT("/Script/Engine.StaticMesh'/Game/KBD/Pallet/PalletBroken.PalletBroken'"));
	if (BrokenMeshObj.Succeeded())
	{
		BrokenMesh->SetupAttachment(RootComponent);
		BrokenMesh->SetVisibility(false);
		BrokenMesh->SetStaticMesh(BrokenMeshObj.Object);
	}
	
	if (Mesh)
	{
		AttachPoint1 = CreateDefaultSubobject<UInteractionPoint>(TEXT("AttachPoint1"));
		AttachPoint1->SetupAttachment(Mesh, TEXT("joint_Attach1"));
		// AttachPoint1->SetRelativeRotation(FRotator(-90, 0, -90));
		AttachPoint1->OnInteraction.AddDynamic(this, &APallet::OnInteraction);
		// AttachPoint1->OnStopInteraction.AddDynamic(this, &APallet::OnStopInteraction);
		AttachPoint1->InteractionMode = EInteractionMode::EIM_CamperOnly;
		AttachPoint1->bCanInteract = true;
		
		AttachPoint2 = CreateDefaultSubobject<UInteractionPoint>(TEXT("AttachPoint2"));
		AttachPoint2->SetupAttachment(Mesh, TEXT("joint_Attach2"));
		// AttachPoint2->SetRelativeRotation(FRotator(-90, 0, -90));
		AttachPoint2->OnInteraction.AddDynamic(this, &APallet::OnInteraction);
		// AttachPoint2->OnStopInteraction.AddDynamic(this, &APallet::OnStopInteraction);
		AttachPoint2->InteractionMode = EInteractionMode::EIM_CamperOnly;
		AttachPoint2->bCanInteract = true;
	}
}

// Called when the game starts or when spawned
void APallet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APallet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APallet::OnInteraction(UInteractionPoint* Point, AActor* OtherActor)
{
	Point->AttachActor(OtherActor, 0, false);
	if (auto* Camper = Cast<ACamper>(OtherActor))
	{
		if (Point->InteractionMode == EInteractionMode::EIM_CamperOnly)
		{
			// 생존자에게 판자 내리는 애니메이션 실행하라고 함.
			AttachPoint1->bCanInteract = false;
			AttachPoint2->bCanInteract = false;
		}
		else
		{
			// 생존자에게 판자 슬라이딩 애니메이션 실행하라고 함.
		}
	}
	else if (auto* Slasher = Cast<ACanival>(OtherActor))
	{
		// 살인마에게 판자 부수는 애니메이션 실행하라고 함.
		AttachPoint1->bCanInteract = false;
		AttachPoint2->bCanInteract = false;
	}
}

void APallet::FallGround()
{
	OnFallGround.Broadcast();
	
	AttachPoint1->bCanInteract = true;
	AttachPoint1->InteractionMode = EInteractionMode::EIM_Both;
	AttachPoint1->DetachActor();
	
	AttachPoint2->bCanInteract = true;
	AttachPoint2->InteractionMode = EInteractionMode::EIM_Both;
	AttachPoint2->DetachActor();

	// TODO: 만약 판자 근처에 살인마가 있었다면 기절 적용
}

void APallet::Broken()
{
	AttachPoint1->bCanInteract = false;
	AttachPoint1->InteractionMode = EInteractionMode::EIM_None;
	AttachPoint1->DetachActor();
	
	AttachPoint2->bCanInteract = false;
	AttachPoint2->InteractionMode = EInteractionMode::EIM_None;
	AttachPoint2->DetachActor();

	Mesh->SetVisibility(false);
	BrokenMesh->SetVisibility(true);
}
