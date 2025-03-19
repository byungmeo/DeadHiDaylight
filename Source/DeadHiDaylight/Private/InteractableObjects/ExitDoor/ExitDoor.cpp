// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor.h"


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
}

// Called when the game starts or when spawned
void AExitDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExitDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExitDoor::OnInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
}

void AExitDoor::OnStopInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
}

