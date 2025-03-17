// Fill out your copyright notice in the Description page of Project Settings.


#include "Meathook.h"

#include "MeathookAnim.h"


// Sets default values
AMeathook::AMeathook()
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
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KBD/Meathook/ABP_Meathook.ABP_Meathook_C'"));
	if (AnimClass.Succeeded())
	{
		Mesh->SetAnimInstanceClass(AnimClass.Class);
	}
}

// Called when the game starts or when spawned
void AMeathook::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeathook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

