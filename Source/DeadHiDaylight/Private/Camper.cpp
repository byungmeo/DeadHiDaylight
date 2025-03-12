


#include "DeadHiDaylight/Public/Camper.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ACamper::ACamper()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACamper::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [this](){ 
	auto* pc = Cast<APlayerController>(Controller);
	if (pc) {
		//그 객체를 이용해서 EnhanceInputLocalPlayerSubSystem을 가져온다.
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys) {
			subSys->AddMappingContext(IMC_Camper, 0);
		}
	}
}, 0.1f, false);

	moveComp = GetCharacterMovement();
	moveComp->MaxWalkSpeed = moveSpeed;
}

// Called every frame
void ACamper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FVector P0 = GetActorLocation();
	FVector vt = direction * moveSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);*/
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::Zero();
}

// Called to bind functionality to input
void ACamper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACamper::CamperMove);
		input->BindAction(IA_Run, ETriggerEvent::Started, this, &ACamper::Run);
		input->BindAction(IA_Run, ETriggerEvent::Completed, this, &ACamper::Run);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACamper::Lookup);
		input->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ACamper::Turn);
	}
}

void ACamper::CamperMove(const FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();
	direction.X = dir.X;
	direction.Y = dir.Y;
	// direction.Normalize();
}

void ACamper::Lookup(const struct FInputActionValue& value)
{
	float v = value.Get<float>();
	AddControllerPitchInput(v);
}

void ACamper::Turn(const struct FInputActionValue& value)
{
	float v = value.Get<float>();
	AddControllerYawInput(v);
}

void ACamper::Run(const struct FInputActionValue& value)
{
	auto movement = moveComp;
	if (movement->MaxWalkSpeed > moveSpeed) movement->MaxWalkSpeed = moveSpeed;
	else movement->MaxWalkSpeed = maxSpeed;
}
