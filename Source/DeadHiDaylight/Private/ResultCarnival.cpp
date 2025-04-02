// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultCarnival.h"

#include "Camera/CameraComponent.h"


// Sets default values
AResultCarnival::AResultCarnival()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KHA/Carnival/Character/Carnival.Carnival'"));
	if (MeshObj.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshObj.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, 0, 0));
	}
	
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KBD/ResultCarnival/ABP_ResultCarnival.ABP_ResultCarnival_C'"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->SetRelativeLocation(FVector(-444.299101, 509.684589, 385.0));
	CameraComponent->SetRelativeRotation(FRotator(-20, -69.999999, 0));
	
	ChainSaw = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChainSaw"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> ChainSawObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KHA/Carnival/Weapon/ChainSaw/ChainSaw.ChainSaw'"));
	if (ChainSawObj.Succeeded())
	{
		ChainSaw->SetSkeletalMesh(ChainSawObj.Object);
	}
	ChainSaw->SetupAttachment(GetMesh(), TEXT("joint_WeaponLT_01"));
	ChainSaw->SetRelativeRotation(FRotator(0,0,-90));
	ChainSaw->SetRelativeScale3D(FVector(0.44f,0.44f,0.44f));
}

// Called when the game starts or when spawned
void AResultCarnival::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResultCarnival::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AResultCarnival::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

