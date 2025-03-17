// Fill out your copyright notice in the Description page of Project Settings.


#include "Canival.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"

// Sets default values
ACanival::ACanival()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/KHA/Carnival/Inputs/IMC_Canival.IMC_Canival'"));
	if (TempIMC.Succeeded())
	{
		imc_carnival = TempIMC.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("/Script/EnhancedInput.InputAction'/Game/KHA/Carnival/Inputs/IA_Move.IA_Move'"));
	if (TempIAMove.Succeeded())
	{
		ia_move = TempIAMove.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAKick(TEXT("/Script/EnhancedInput.InputAction'/Game/KHA/Carnival/Inputs/IA_Kick.IA_Kick'"));
	if (TempIAKick.Succeeded())
	{
		ia_kick = TempIAKick.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempKickMontage(TEXT("/Script/Engine.AnimMontage'/Game/KHA/Carnival/Character/Animations/Obstacles/CA_Destroy_Pallet_Montage.CA_Destroy_Pallet_Montage'"));
	if (TempKickMontage.Succeeded())
	{
		MyAnimMontage = TempKickMontage.Object;
	}
}

// Called when the game starts or when spawned
void ACanival::BeginPlay()
{
	Super::BeginPlay();
	
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSystem)
		{
			subSystem->AddMappingContext(imc_carnival, 0);
		}
	}
}



// Called every frame
void ACanival::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACanival::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pi = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (pi)
	{
		pi->BindAction(ia_move, ETriggerEvent::Triggered,this, &ACanival::Move);
		pi->BindAction(ia_kick, ETriggerEvent::Triggered,this, &ACanival::PlayKick);
	}
}

void ACanival::Move(const struct FInputActionValue& inputValue)
{
	// input is a Vector2D
	FVector2D MovementVector = inputValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
	/*FVector2d dir = inputValue.Get<FVector2d>();
	direction.X = dir.Y;
	direction.Y = dir.X;*/
}

void ACanival::Kick(const struct FInputActionValue& inputValue)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("인풋 스페이스"));
		PlayKick(inputValue);
	}
	
}
void ACanival::PlayKick(const FInputActionValue& inputValue)
{
	// MyAnimMontage가 유효한지 확인한 후, 애니메이션 몽타주 재생
	if (MyAnimMontage)
	{
		PlayAnimMontage(MyAnimMontage);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("킥"));
		}
	}
	
}