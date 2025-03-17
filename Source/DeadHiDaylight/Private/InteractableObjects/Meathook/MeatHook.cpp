// Fill out your copyright notice in the Description page of Project Settings.


#include "MeatHook.h"

#include "Camper.h"
#include "InteractionPoint.h"


// Sets default values
AMeatHook::AMeatHook()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KBD/Meathook/DeerHanger.DeerHanger'"));
	if (MeshObj.Succeeded())
	{
		Mesh->SetSkeletalMeshAsset(MeshObj.Object);
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KBD/Meathook/ABP_Meathook.ABP_MeatHook_C'"));
	if (AnimClass.Succeeded())
	{
		Mesh->SetAnimInstanceClass(AnimClass.Class);
	}

	if (Mesh)
	{
		CamperPoint = CreateDefaultSubobject<UInteractionPoint>(TEXT("CamperPoint"));
		CamperPoint->SetupAttachment(Mesh, TEXT("joint_CamperAttach"));
		CamperPoint->SetRelativeRotation(FRotator(-90, 0, -90));
		CamperPoint->OnInteraction.AddDynamic(this, &AMeatHook::OnInteraction);
		CamperPoint->OnStopInteraction.AddDynamic(this, &AMeatHook::OnStopInteraction);
	}
}

// Called when the game starts or when spawned
void AMeatHook::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMeatHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeatHook::OnInteraction(UInteractionPoint* Point, AActor* OtherActor)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMeathook::OnInteraction Survivor"));
		
		// TODO: Case1 : 생존자가 갈고리에 걸려있는 생존자를 구하는 경우
		
		// TODO: Case2 : 갈고리에 걸려있는 생존자가 스스로 탈출을 시도하는 경우
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMeathook::OnInteraction Slasher"));
		
		// TODO: 살인마가 생존자를 업은 상태에서 갈고리에 걸려고 하는 경우
		
	}
}

void AMeatHook::OnStopInteraction(UInteractionPoint* Point, AActor* OtherActor)
{
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMeathook::OnInteraction Survivor"));
		
		// TODO: Case1 : 생존자가 갈고리에 걸려있는 생존자를 구하다가 중단하는 경우
		
		// TODO: Case2 : 갈고리에 걸려있는 생존자가 스스로 탈출을 시도하다가 중단하는 경우
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMeathook::OnInteraction Slasher"));
		// 중단 불가
	}
}