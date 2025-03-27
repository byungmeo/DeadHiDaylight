// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCamper.h"
#include "Camper.h"
#include "CamperAnimInstance.h" 

// Sets default values
ATestCamper::ATestCamper()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestCamper::BeginPlay()
{
	Super::BeginPlay();
	
	if (!Anim)
	{
		Anim = Cast<UCamperAnimInstance>(GetMesh()->GetAnimInstance());
	}
    
	if (Anim)
	{
		Anim->bCrawl = true;
		UE_LOG(LogTemp, Warning, TEXT("죽은상태"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TestCamper BeginPlay: Animation instance is null."));
	}
	
}

// Called every frame
void ATestCamper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATestCamper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

