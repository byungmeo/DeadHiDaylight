


#include "DeadHiDaylight/Public/Camper.h"

#include "CamperAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionPoint.h"
#include "Camera/CameraComponent.h"
#include "CamperComps/PerksComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	springArmComp->TargetArmLength = 450;
	springArmComp->bUsePawnControlRotation = true;
	
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->SetRelativeRotation(FRotator(-10, 0, 0));

	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	perksComp = CreateDefaultSubobject<UPerksComponent>(TEXT("PerksComp"));
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
	
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		GetDamage();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
	{
		Anim->bInjure = false;
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
	{
		// 쓰러진 상태에서 돌아갈 때 Crawlhealing이 끝나면
		curHP = maxHP;
		Anim->bCrawl = false;
		Anim->bInjure = false;
		moveComp->MaxWalkSpeed = moveSpeed;
		UE_LOG(LogTemp, Warning, TEXT("%d"), Anim->bInjure);
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
		input->BindAction(IA_Repair, ETriggerEvent::Started, this, &ACamper::CheckInteractPoint);
		input->BindAction(IA_Repair, ETriggerEvent::Completed, this, &ACamper::Test);

		perksComp->SetupInputBinding(input);
	}
}

void ACamper::CamperMove(const FInputActionValue& value)
{
	if (Anim->bStartRepair || Anim->bSelfHealing || (Anim->bCrawl && Anim->bHitCrawl == false)) return;
	
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
	if (Anim->bSelfHealing || Anim->bCrawl) return; // 자가 치유 중이라면 리턴
	
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
	if (Anim->bSelfHealing || Anim->bCrawl) return; // 자가 치유 중이라면 리턴
	if (Anim) Anim->IsCrouch(true);
	
	if (Anim)
	{
		springArmComp->SetRelativeLocation(FVector(0, 0, 160));
		moveComp->MaxWalkSpeed = crouchSpeed * 2;
	}
	// UE_LOG(LogTemp, Warning, TEXT("ACamper::StartCrouch %f"), moveComp->MaxWalkSpeed);
}

void ACamper::End_Crouch(const struct FInputActionValue& value)
{
	if (Anim) Anim->IsCrouch(false);
	
	if (Anim)
	{
		springArmComp->SetRelativeLocation(FVector(0, 0, 210));
		moveComp->MaxWalkSpeed = moveSpeed * 2;
	}
	// UE_LOG(LogTemp, Warning, TEXT("ACamper::EndCrouch %f"), moveComp->MaxWalkSpeed);
}
void ACamper::CheckInteractPoint()
{
	if (Anim->bSelfHealing || Anim->bCrawl) return; // 자가 치유 중이라면 리턴
	
	// InteractionPoint 찾는 Trace
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<FHitResult> OutHits;
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiByProfile(
		GetWorld(),
		GetMovementComponent()->GetFeetLocation(),
		GetMovementComponent()->GetFeetLocation(),
		500,
		TEXT("InteractionPoint"),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		OutHits,
		true
	);
	if (bHit)
	{
		bFindPoints = true;
		for (const auto HitResult : OutHits)
		{
			
			if (auto interact = Cast<UInteractionPoint>(HitResult.GetComponent()))
			{
				if (Anim == nullptr || Anim->bStartRepair) return;
				
				UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *HitResult.GetComponent()->GetName(), Anim->bStartRepair);
				interact->Interaction(this);
				SaveInteract = interact;
				break;
			}
		}
	}
	else
	{
		bFindPoints = false;
	}
}

void ACamper::StartRepair()
{
	if (Anim == nullptr || Anim->bStartRepair || Anim->bSelfHealing || Anim->bCrawl)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camper : StartRepair : Anim : nullptr"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("발전기 수리 시작"));
	// 시작 애니메이션 몽타주 실행
	Anim->PlayRepairAnimation(TEXT("GenIn"));
}

void ACamper::EndRepair()
{
	if (Anim == nullptr || Anim->bEndRepair == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camper : EndRepair : Anim : nullptr"));
		return;
	}
	Anim->bEndRepair = false;
	UE_LOG(LogTemp, Warning, TEXT("발전기 수리 중단/종료"));
	
	// 다시 애니메이션 idle로 바꾸고 wsad 움직일 수 있게 변경
	Anim->PlayRepairAnimation(TEXT("GenOut"));
}


void ACamper::Test()
{
	if (SaveInteract)
	{
		SaveInteract->StopInteraction(this);
	}
}

void ACamper::GetDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("Camper : GetDamage : %f "), curHP);
	if (curHP > 1)
	{
		// HP를 줄이고
		--curHP;
		UE_LOG(LogTemp, Warning, TEXT("Camper : GetDamage : %f "), curHP);
		// 다친 상태로 변경하고
		Anim->bInjure = true;
		// 이전 속도를 저장
		beforeSpeed = moveComp->MaxWalkSpeed;
		// 다쳤을 때 2초동안 스피드가 2배 증가
		moveComp->MaxWalkSpeed = moveComp->MaxWalkSpeed * 2;
		
		// 2초 후 다시 이전 속도로 복귀
		FTimerHandle hitTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(hitTimerHandle, this, &ACamper::HitSpeedTimer, 2.0f, false);
	}
	else
	{
		Crawling();
	}
}

void ACamper::HitSpeedTimer()
{
	if (Anim->bCrawl) return;
	moveComp->MaxWalkSpeed = beforeSpeed;
}

void ACamper::Crawling()
{
	Anim->HitCrawl();
	Anim->bCrawl = true;
	moveComp->MaxWalkSpeed = crawlSpeed;
}

void ACamper::StartUnLock()
{
	if (Anim == nullptr || Anim->bUnLocking) return;

	Anim->PlayUnLockAnimation(TEXT("StartUnLock"));
}

void ACamper::EndUnLock()
{
	if (Anim == nullptr) return;
	
	Anim->PlayUnLockAnimation(TEXT("OpenDoor"));
}
