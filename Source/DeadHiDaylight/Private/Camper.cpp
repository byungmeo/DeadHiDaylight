


#include "DeadHiDaylight/Public/Camper.h"

#include "CamperAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ACamper::ACamper()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

// Called when the game starts or when spawned
void ACamper::BeginPlay()
{
	Super::BeginPlay();

	Anim = Cast<UCamperAnimInstance>(GetMesh()->GetAnimInstance());

	// 시작 시 IMC 세팅
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

	// 캐릭터 움직임 컴포넌트 세팅
	moveComp = GetCharacterMovement();
	moveComp->MaxWalkSpeed = moveSpeed;
}

// Called every frame
void ACamper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// InteractionPoint 찾는 Trace
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<FHitResult> OutHits;
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiByProfile(
		GetWorld(),
		GetMovementComponent()->GetFeetLocation(),
		GetMovementComponent()->GetFeetLocation(),
		200,
		TEXT("InteractionPoint"),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		OutHits,
		true
	);
	if (bHit)
	{
		for (const auto HitResult : OutHits)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetComponent()->GetName());
		}
	}
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
		input->BindAction(IA_Crouch, ETriggerEvent::Started, this, &ACamper::Start_Crouch);
		input->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &ACamper::End_Crouch);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACamper::Look);
	}
}

void ACamper::CamperMove(const FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, dir.Y);
	AddMovementInput(RightDirection, dir.X);
}

void ACamper::Look(const struct FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	AddControllerYawInput(v.X);
	AddControllerPitchInput(v.Y);
}

void ACamper::Run(const struct FInputActionValue& value)
{
	if (Anim)
	{
		Anim->IsRun();
	}
	if (Anim->bRun) moveComp->MaxWalkSpeed = maxSpeed * 2;
	else moveComp->MaxWalkSpeed = moveSpeed * 2;

	// UE_LOG(LogTemp, Warning, TEXT("ACamper::Run %f"), movement->MaxWalkSpeed);
}

void ACamper::Start_Crouch(const struct FInputActionValue& value)
{
	if (Anim) Anim->IsCrouch(true);
	
	if (Anim)
	{
		moveComp->MaxWalkSpeed = crouchSpeed * 2;
	}
	// UE_LOG(LogTemp, Warning, TEXT("ACamper::StartCrouch %f"), moveComp->MaxWalkSpeed);
}

void ACamper::End_Crouch(const struct FInputActionValue& value)
{
	if (Anim) Anim->IsCrouch(false);
	
	if (Anim)
	{
		moveComp->MaxWalkSpeed = moveSpeed * 2;
	}
	// UE_LOG(LogTemp, Warning, TEXT("ACamper::EndCrouch %f"), moveComp->MaxWalkSpeed);
}

void ACamper::StartRepair()
{
	UE_LOG(LogTemp, Warning, TEXT("발전기 수리 시작"));

	// TODO: 발전기 수리 시작 시 해야할 일 작성
}

void ACamper::EndRepair()
{
	UE_LOG(LogTemp, Warning, TEXT("발전기 수리 중단/종료"));

	// TODO: 발전기 수리 종료 시 해야할 일 작성
}
