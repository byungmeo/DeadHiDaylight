// Fill out your copyright notice in the Description page of Project Settings.


#include "Canival.h"

#include "EnhancedInputComponent.h"

// Sets default values
ACanival::ACanival()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("/Game/DBDAssets/Carnival/Inputs/IA_Move.IA_Move"));
	if (TempIAMove.Succeeded())
	{
		ia_move = TempIAMove.Object;
	}
}

// Called when the game starts or when spawned
void ACanival::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ACanival::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACanival::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	//Super::SetupInputBinding(PlayerInputComponent);
	
	PlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered,this, &ACanival::Move);
		
	
}

void ACanival::Move(const struct FInputActionValue& inputValue)
{
	FVector2d dir = inputValue.Get<FVector2d>();
	direction.X = dir.Y;
	direction.Y = dir.X;
}