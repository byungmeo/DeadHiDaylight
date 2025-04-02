// Fill out your copyright notice in the Description page of Project Settings.


#include "CamperSpawnPoint.h"


// Sets default values
ACamperSpawnPoint::ACamperSpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

// Called when the game starts or when spawned
void ACamperSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACamperSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

