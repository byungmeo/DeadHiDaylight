// Fill out your copyright notice in the Description page of Project Settings.


#include "SlasherSpawnPoint.h"


// Sets default values
ASlasherSpawnPoint::ASlasherSpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
}

// Called when the game starts or when spawned
void ASlasherSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlasherSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

