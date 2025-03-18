


#include "DeadHiDaylight/Public/Camper.h"

#include "CamperAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "Generator.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ACamper::ACamper()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	glassesComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("glassesComp"));
	glassesComp->SetupAttachment(GetMesh(), TEXT("Glasses"));
	glassesComp->SetRelativeLocation(FVector(2.2f, -11, 0));
	glassesComp->SetRelativeScale3D(FVector(0.72f));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempGlassesMesh(TEXT("/Script/Engine.StaticMesh'/Game/JS/Assets/Camper/Character/Claudette/Acc/Glasses/CM_Glasses_Acc01_REF.CM_Glasses_Acc01_REF'"));

	if (tempGlassesMesh.Succeeded())
	{
		glassesComp->SetStaticMesh(tempGlassesMesh.Object);
	}
	hairComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("hairComp"));
	hairComp->SetupAttachment(GetMesh(), TEXT("Hair"));
	hairComp->SetRelativeLocationAndRotation(FVector(2.84f, -9, 1), FRotator(80, 180, 100));
	hairComp->SetRelativeScale3D(FVector(0.35f));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempHairMesh(TEXT("/Script/Engine.StaticMesh'/Game/JS/Assets/Camper/Character/Claudette/Acc/Hair/CM_Hair_Acc01_REF.CM_Hair_Acc01_REF'"));

	if (tempHairMesh.Succeeded())
	{
		hairComp->SetStaticMesh(tempHairMesh.Object);
	}
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 0, 210));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;
	
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->SetRelativeRotation(FRotator(-10, 0, 0));

	
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
		input->BindAction(IA_Repair, ETriggerEvent::Started, this, &ACamper::StartRepair);
		input->BindAction(IA_Repair, ETriggerEvent::Completed, this, &ACamper::EndRepair);
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
	if (Anim == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camper : StartRepair : Anim : nullptr"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("발전기 수리 시작"));
	// OverlappedGeneratorSlot->Generator->Interact(this);
	// 움직임 wsad 멈추고 카메라만 회전
	// 애니메이션 실행
	Anim->PlayRepairAnimation(TEXT("GenIn"));
	// 수리 중인데 마우스를 땠을 때 호출하는 함수
	// OverlappedGeneratorSlot->Generator->NotifyEndRepair(this);

	// TODO: 발전기 수리 시작 시 해야할 일 작성
}

void ACamper::EndRepair()
{
	if (Anim == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camper : EndRepair : Anim : nullptr"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("발전기 수리 중단/종료"));
	// 다시 애니메이션 idle로 바꾸고 wsad 움직일 수 있게 변경
	Anim->PlayRepairAnimation(TEXT("GenOut"));

	// TODO: 발전기 수리 종료 시 해야할 일 작성
}
