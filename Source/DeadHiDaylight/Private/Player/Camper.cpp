// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Camper.h"

#include "CamperAnimInstance.h"
#include "Canival.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "Generator.h"
#include "InputMappingContext.h"
#include "InteractionPoint.h"
#include "MeatHook.h"
#include "Pallet.h"
#include "SacrificeCommonHUD.h"
#include "SacrificeGameState.h"
#include "SacrificePlayerController.h"
#include "SacrificePlayerState.h"
#include "Camera/CameraComponent.h"
#include "CamperComps/CamperFSM.h"
#include "CamperComps/PerksComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"


// Sets default values
ACamper::ACamper()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(212.173325f);
	GetCapsuleComponent()->SetCapsuleRadius(70);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -210), FRotator(0, -90, 0));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/JS/Assets/Camper/Character/Claudette/Claudette.Claudette'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(tempMesh.Object);
	
		glassesComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("glassesComp"));
		ConstructorHelpers::FObjectFinder<UStaticMesh> tempGlassesMesh(TEXT("/Script/Engine.StaticMesh'/Game/JS/Assets/Camper/Character/Claudette/Acc/Glasses/CM_Glasses_Acc01_REF.CM_Glasses_Acc01_REF'"));
		if (tempGlassesMesh.Succeeded())
		{
			glassesComp->SetStaticMesh(tempGlassesMesh.Object);
			glassesComp->SetupAttachment(GetMesh(), TEXT("Glasses"));
			glassesComp->SetRelativeLocation(FVector(2.2f, -11, 0));
			glassesComp->SetRelativeScale3D(FVector(0.72f));
		}
	
		hairComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("hairComp"));
		ConstructorHelpers::FObjectFinder<UStaticMesh> tempHairMesh(TEXT("/Script/Engine.StaticMesh'/Game/JS/Assets/Camper/Character/Claudette/Acc/Hair/CM_Hair_Acc01_REF.CM_Hair_Acc01_REF'"));
		if (tempHairMesh.Succeeded())
		{
			hairComp->SetStaticMesh(tempHairMesh.Object);
			hairComp->SetupAttachment(GetMesh(), TEXT("Hair"));
			hairComp->SetRelativeLocationAndRotation(FVector(2.84f, -9, 1), FRotator(80, 180, 100));
			hairComp->SetRelativeScale3D(FVector(0.35f));
		}
	}
	
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 0, 210));
	springArmComp->TargetArmLength = 450;
	springArmComp->bUsePawnControlRotation = true;
	
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->SetRelativeRotation(FRotator(-10, 0, 0));

	
	// IA
	static const ConstructorHelpers::FObjectFinder<UInputMappingContext> tempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/JS/Input/IMC_FirstCamper.IMC_FirstCamper'"));
	if (tempIMC.Succeeded())
	{
		IMC_Camper = tempIMC.Object;
	}
	static const ConstructorHelpers::FObjectFinder<UInputAction> tempIAMove(TEXT("/Script/EnhancedInput.InputAction'/Game/JS/Input/IA/IA_CamperMove.IA_CamperMove'"));
	if (tempIAMove.Succeeded())
	{
		IA_Move = tempIAMove.Object;
	}
	static const ConstructorHelpers::FObjectFinder<UInputAction> tempIALook(TEXT("/Script/EnhancedInput.InputAction'/Game/JS/Input/IA/IA_Look.IA_Look'"));
	if (tempIALook.Succeeded())
	{
		IA_Look = tempIALook.Object;
	}
	static const ConstructorHelpers::FObjectFinder<UInputAction> tempIARun(TEXT("/Script/EnhancedInput.InputAction'/Game/JS/Input/IA/IA_Run.IA_Run'"));
	if (tempIARun.Succeeded())
	{
		IA_Run = tempIARun.Object;
	}
	static const ConstructorHelpers::FObjectFinder<UInputAction> tempIACrouch(TEXT("/Script/EnhancedInput.InputAction'/Game/JS/Input/IA/IA_Crouch.IA_Crouch'"));
	if (tempIACrouch.Succeeded())
	{
		IA_Crouch = tempIACrouch.Object;
	}
	static const ConstructorHelpers::FObjectFinder<UInputAction> tempIARepair(TEXT("/Script/EnhancedInput.InputAction'/Game/JS/Input/IA/IA_Repair.IA_Repair'"));
	if (tempIARepair.Succeeded())
	{
		IA_Repair = tempIARepair.Object;
	}
	static const ConstructorHelpers::FObjectFinder<UInputAction> tempIAUnLock(TEXT("/Script/EnhancedInput.InputAction'/Game/JS/Input/IA/IA_UnLock.IA_UnLock'"));
	if (tempIAUnLock.Succeeded())
	{
		IA_UnLock = tempIAUnLock.Object;
	}
	
	// Sound
	static const ConstructorHelpers::FObjectFinder<USoundCue> tempLeftSound(TEXT("/Script/Engine.SoundCue'/Game/JS/Assets/Sound/run/SC_LeftFoot.SC_LeftFoot'"));
	if (tempLeftSound.Succeeded())
	{
		leftFootCue = tempLeftSound.Object;
	}
	static const ConstructorHelpers::FObjectFinder<USoundAttenuation> AttenuationLeftObj(TEXT("/Script/Engine.SoundAttenuation'/Game/JS/Assets/Sound/run/SA_LeftFoot.SA_LeftFoot'"));
	if (AttenuationLeftObj.Succeeded())
	{
		leftFootAttenuation = AttenuationLeftObj.Object;
	}
	
	static const ConstructorHelpers::FObjectFinder<USoundCue> tempRightSound(TEXT("/Script/Engine.SoundCue'/Game/JS/Assets/Sound/run/SC_RightFoot.SC_RightFoot'"));
	if (tempRightSound.Succeeded())
	{
		rightFootCue = tempRightSound.Object;
	}
	static const ConstructorHelpers::FObjectFinder<USoundAttenuation> AttenuationRightObj(TEXT("/Script/Engine.SoundAttenuation'/Game/JS/Assets/Sound/run/SA_RightFoot.SA_RightFoot'"));
	if (AttenuationRightObj.Succeeded())
	{
		rightFootAttenuation = AttenuationRightObj.Object;
	}
	
	static const ConstructorHelpers::FObjectFinder<USoundCue> tempinjureCue(TEXT("/Script/Engine.SoundCue'/Game/JS/Assets/Sound/injurewav/SC_Injure.SC_Injure'"));
	if (tempinjureCue.Succeeded())
	{
		injureCue = tempinjureCue.Object;
	}
	static const ConstructorHelpers::FObjectFinder<USoundAttenuation> tempinjureAttenuation(TEXT("/Script/Engine.SoundAttenuation'/Game/JS/Assets/Sound/injurewav/SA_Injure.SA_Injure'"));
	if (tempinjureAttenuation.Succeeded())
	{
		injureAttenuation = tempinjureAttenuation.Object;
	}

	static const ConstructorHelpers::FObjectFinder<USoundCue> tempInjureScreamCue(TEXT("/Script/Engine.SoundCue'/Game/JS/Assets/Sound/hit/SC_Scream.SC_Scream'"));
	if (tempInjureScreamCue.Succeeded())
	{
		injuredScreamCue = tempInjureScreamCue.Object;
	}
	static const ConstructorHelpers::FObjectFinder<USoundAttenuation> tempInjureScreamAttenuation(TEXT("/Script/Engine.SoundAttenuation'/Game/JS/Assets/Sound/hit/SA_Scream.SA_Scream'"));
	if (tempInjureScreamAttenuation.Succeeded())
	{
		injuredScreamAttenuation = tempInjureScreamAttenuation.Object;
	}

	static const ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/JS/Blueprints/Animation/MTG_HitCrawl.MTG_HitCrawl'"));
	if (tempMontage.Succeeded())
	{
		hitcrawlMontage = tempMontage.Object;
	}

	static const ConstructorHelpers::FObjectFinder<USoundCue> tempHookInCue(TEXT("/Script/Engine.SoundCue'/Game/JS/Assets/Sound/hook/SC_HookIn.SC_HookIn'"));
	if (tempHookInCue.Succeeded())
	{
		hookInCue = tempHookInCue.Object;
	}
	static const ConstructorHelpers::FObjectFinder<USoundAttenuation> tempHookInAttenuation(TEXT("/Script/Engine.SoundAttenuation'/Game/JS/Assets/Sound/hook/SA_HookIn.SA_HookIn'"));
	if (tempHookInAttenuation.Succeeded())
	{
		hookInAttenuation = tempHookInAttenuation.Object;
	}
	// DieSound
	static const ConstructorHelpers::FObjectFinder<USoundCue> tempDieCue(TEXT("/Script/Engine.SoundCue'/Game/JS/Assets/Sound/die/SC_Die.SC_Die'"));
	if (tempDieCue.Succeeded())
	{
		dieCue = tempDieCue.Object;
	}
	static const ConstructorHelpers::FObjectFinder<USoundAttenuation> tempDieAttenuation(TEXT("/Script/Engine.SoundAttenuation'/Game/JS/Assets/Sound/die/SA_Die.SA_Die'"));
	if (tempDieAttenuation.Succeeded())
	{
		dieAttenuation = tempDieAttenuation.Object;
	}
	
	// CharacterMovement 컴포넌트
	GetCharacterMovement()->bOrientRotationToMovement = true;

	perksComp = CreateDefaultSubobject<UPerksComponent>(TEXT("perksComp")); // 퍽 컴포넌트
	camperFSMComp = CreateDefaultSubobject<UCamperFSM>(TEXT("camperFSMComp")); // FSM 컴포넌트

	springArmComp->bEnableCameraLag = true;
	springArmComp->CameraLagSpeed = 10;
	bUseControllerRotationYaw = false;
	
	// 캐릭터 움직임 컴포넌트 세팅
	moveComp = GetCharacterMovement();
	moveComp->MaxWalkSpeed = moveSpeed;

	CrawlPoint = CreateDefaultSubobject<UInteractionPoint>(TEXT("CrawlPoint"));
	CrawlPoint->SetupAttachment(GetMesh(), TEXT("joint_Pelvis_01"));
	CrawlPoint->OnInteraction.AddDynamic(this, &ACamper::OnInteraction);
	CrawlPoint->OnStopInteraction.AddDynamic(this, &ACamper::OnStopInteraction);
	// TODO: 생존자가 치료해서 살릴 수 있게도 바꿔야 함.
	CrawlPoint->InteractionMode = EInteractionMode::EIM_Both;
	CrawlPoint->bCanInteract = false;
	
	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/JS/Blueprints/Animation/ABP_Player.ABP_Player_C'"));
	if (tempAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnimInstance.Class);
	}
}

void ACamper::BeginPlay()
{
	Super::BeginPlay();
	
	if (const auto* pc = Cast<APlayerController>(Controller)) {
		//그 객체를 이용해서 EnhanceInputLocalPlayerSubSystem을 가져온다.
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys) {
			subSys->AddMappingContext(IMC_Camper, 0);
		}
	}
	Anim = Cast<UCamperAnimInstance>(GetMesh()->GetAnimInstance());

	if (HasAuthority())
	{
		FTimerHandle FindPointTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(FindPointTimerHandle, this, &ACamper::ServerOnly_FindInteractionPoint, 0.1f, true);
	}
}

void ACamper::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 서버에서 초기화
	userState = Cast<ASacrificePlayerState>(GetPlayerState());
	if (userState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] PlayerState successfully casted!"));
	}
}

void ACamper::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 클라이언트에서 초기화
	userState = Cast<ASacrificePlayerState>(GetPlayerState());
	ServerRPC_InitPlayerState(userState);
	if (userState) UE_LOG(LogTemp, Warning, TEXT("[Client] PlayerState successfully casted!"));
	
}

void ACamper::ServerRPC_InitPlayerState_Implementation(class ASacrificePlayerState* InState)
{
	NET_LOG(LogTemp, Warning, TEXT("ServerRPC_InitPlayerState_Implementation"));
	userState = InState;
}

// Called every frame
void ACamper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) HealingTimingCheck(DeltaTime);
	
	// if (camperFSMComp->curInteractionState == ECamperInteraction::ECI_ResueHooking)
	// {
	// 	// CheckRescueTime();
	// 	// 갈고리 자가 탈출 테스트
	// 	testCheckTime += DeltaTime;
	// 	UE_LOG(LogTemp, Warning, TEXT("%f"), testCheckTime);
	// 	if (testCheckTime > 3.0f)
	// 	{
	// 		SetHealthState(ECamperHealth::ECH_Injury);
	// 		Hooking(TEXT("HookFree"));
	// 		btest = false;
	// 		testCheckTime = 0.0f;
	// 	}
	// 	
	// }
	
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		GetDamage(TEXT(""));
	}
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two))
	{
		PickUp();
	}
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three))
	{
		// 쓰러진 상태에서 돌아갈 때 Crawlhealing이 끝나면
		ServerRPC_HealthCheck();
		SetStanceState(ECamperStanceState::ECSS_Idle);
	}
	// Hook 걸리는 거 테스트 용
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four))
	{
		Hooking(TEXT("HookIn"));
	}
	// Hook 자가 탈출 테스트 용
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
	{
		Hooking(TEXT("HookStruggle"));
		btest = true;
	}
	// Hook 구해지는 거 테스트
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
	{
		camperFSMComp->curHealthState = ECamperHealth::ECH_Injury;
		Hooking(TEXT("HookRescuedEnd"));
	}
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
	{
		// FVector start = GetActorLocation();
		// FVector end = start + GetActorForwardVector() * 1000;
		// FHitResult hitInfo;
		// FCollisionQueryParams params;
		// params.AddIgnoredActor(this);
		// DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f, 0, 5.0f);
		// bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, params);
		// if (bHit)
		// {
		// 	FString s = hitInfo.GetActor()->GetActorNameOrLabel();
		// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *s);
		// 	if (ACamper* player = Cast<ACamper>(hitInfo.GetActor()))
		// 	{
		// 		RescueHooking(player, TEXT("HookRescueIn"));
		// 		btest = true;
		// 	}
		// } 
		// else
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("No bHit"));
		// }
	}
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
	{
		FailRepair(TEXT("GenFailFT"));
	}
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
	{
		PickUpDrop();
	}
	if (camperFSMComp && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
	{
		PullDownPallet();
	}
	PrintNetLog();

}

// Called to bind functionality to input
void ACamper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACamper::CamperMove);
		input->BindAction(IA_Move, ETriggerEvent::Completed, this, &ACamper::StopCamperMove);
		input->BindAction(IA_Run, ETriggerEvent::Started, this, &ACamper::StartRun);
		input->BindAction(IA_Run, ETriggerEvent::Completed, this, &ACamper::StopRun);
		input->BindAction(IA_Crouch, ETriggerEvent::Started, this, &ACamper::Start_Crouch);
		input->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &ACamper::End_Crouch);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACamper::Look);
		input->BindAction(IA_Repair, ETriggerEvent::Started, this, &ACamper::TryInteraction);
		input->BindAction(IA_Repair, ETriggerEvent::Completed, this, &ACamper::StopInteract);

		if (perksComp)
		{
			perksComp->SetupInputBinding(input);	
		}
	}
}

void ACamper::CamperMove(const FInputActionValue& value)
{
	if (camperFSMComp->curInteractionState == ECamperInteraction::ECI_Repair ||
		camperFSMComp->curInteractionState == ECamperInteraction::ECI_UnLock ||
		camperFSMComp->curInteractionState == ECamperInteraction::ECI_Carry ||
		camperFSMComp->curInteractionState == ECamperInteraction::ECI_Hook ||
		camperFSMComp->curInteractionState == ECamperInteraction::ECI_HookRescue ||
		camperFSMComp->curInteractionState == ECamperInteraction::ECI_Healing ||
		Anim->Montage_IsPlaying(hitcrawlMontage)) return;

	FVector2D moveDirection = value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, moveDirection.Y);
	AddMovementInput(RightDirection, moveDirection.X);

	
	curSpeed = FVector::DotProduct(GetVelocity(), GetActorForwardVector());
	
	if (camperFSMComp->curMoveState == ECamperMoveState::ECS_Run) return;
	SetMovementState(ECamperMoveState::ECS_Move);
}

void ACamper::StopCamperMove(const FInputActionValue& value)
{
	SetMovementState(ECamperMoveState::ECS_NONE);
}

void ACamper::Look(const struct FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	AddControllerYawInput(v.X);
	AddControllerPitchInput(v.Y);
}

// Run Start
void ACamper::StartRun(const struct FInputActionValue& value)
{
	if (camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crouch ||
		camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crawl ||
		Anim->moveSpeed < 3.0f) return;
	
	SetMovementState(ECamperMoveState::ECS_Run);
}
void ACamper::StopRun(const struct FInputActionValue& value)
{
	SetMovementState(ECamperMoveState::ECS_NONE);
}
// Run Stop End

// Crouch Start
void ACamper::Start_Crouch(const struct FInputActionValue& value)
{
	if (camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crawl) return;
	SetStanceState(ECamperStanceState::ECSS_Crouch);
	SetMovementState(ECamperMoveState::ECS_NONE);
}
void ACamper::End_Crouch(const struct FInputActionValue& value)
{
	if (camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crawl) return;
	SetStanceState(ECamperStanceState::ECSS_Idle);
	SetMovementState(ECamperMoveState::ECS_NONE);
}
// Crouch End

void ACamper::SetStanceState(ECamperStanceState NewState)
{
	ServerRPC_SetStanceState(NewState);
}

void ACamper::ServerRPC_SetStanceState_Implementation(ECamperStanceState NewState)
{
	MultiCastRPC_SetStanceState(NewState);
	
	if (userState == nullptr)
    {
    	UE_LOG(LogTemp, Warning, TEXT("userState Not Set"));
    	return;
    }

    FUserState NewUserState = userState->UserState;
    NewUserState.Stance = NewState;
	userState->UserState = NewUserState;
}

void ACamper::MultiCastRPC_SetStanceState_Implementation(ECamperStanceState NewState)
{
	camperFSMComp->curStanceState = NewState;
	
	switch (NewState)
	{
	case ECamperStanceState::ECSS_Idle:
		{
			// curSpeed = 0;
			// moveComp->MaxWalkSpeed = moveSpeed;
			springArmComp->SetRelativeLocation(FVector(0, 0, 210));
			break;
		}
	case ECamperStanceState::ECSS_Crouch:
		{
			springArmComp->SetRelativeLocation(FVector(0, 0, 160));
			break;
		}
	case ECamperStanceState::ECSS_Crawl:
		{
			Anim->PlayHitCrawlAnimation(TEXT("hitCrawl"));
			break;
		}
	}
}

void ACamper::SetMovementState(ECamperMoveState NewState)
{
	if (camperFSMComp->curMoveState == NewState) return;
	ServerRPC_SetMovementState(NewState);
}

void ACamper::ServerRPC_SetMovementState_Implementation(ECamperMoveState NewState)
{
	MultiCastRPC_SetMovementState(NewState);

	if (userState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("userState Not Set"));
		return;
	}

	FUserState NewUserState = userState->UserState;
	NewUserState.Move = NewState;
	userState->UserState = NewUserState;
}

void ACamper::MultiCastRPC_SetMovementState_Implementation(ECamperMoveState NewState)
{
	camperFSMComp->curMoveState = NewState;
	
	FString s = UEnum::GetValueAsString(camperFSMComp->curMoveState);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *s);
	
	switch (NewState)
	{
	case ECamperMoveState::ECS_NONE:
		{
			curSpeed = 0;
			moveComp->StopMovementImmediately();
			break;
		}
	case ECamperMoveState::ECS_Move:
		{
			// Croch 상태
			if (camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crouch)
			{
				curSpeed = crouchSpeed * 2;
				moveComp->MaxWalkSpeed = curSpeed;
			}
			else if (camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crawl)
			{
				curSpeed = crawlSpeed;
				moveComp->MaxWalkSpeed = curSpeed;
			}
			else
			{
				curSpeed = moveSpeed * 2;
				moveComp->MaxWalkSpeed = curSpeed;
			}
			break;
		}
	case ECamperMoveState::ECS_Run:
		{
			if (camperFSMComp->curStanceState != ECamperStanceState::ECSS_Crouch &&
				camperFSMComp->curStanceState != ECamperStanceState::ECSS_Crawl)
			{
				curSpeed = maxSpeed * 2;
				moveComp->MaxWalkSpeed = curSpeed;
			
			}
			break;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("State : %s curSpeed : %f"), *s, curSpeed);
}

void ACamper::ServerOnly_FindInteractionPoint()
{
	if (nullptr != InteractingPoint)
	{
		if (nullptr != NearPoint)
		{
			NearPoint = nullptr;
			ClientRPC_ChangeNearPoint(NearPoint);
		}
		return;
	}
	
	const FVector& StartEnd = GetMovementComponent()->GetFeetLocation();
	auto* Point = UInteractionPoint::FindInteractionPoint(this, StartEnd, StartEnd, EInteractionMode::EIM_CamperOnly);
	if (NearPoint != Point)
	{
		NearPoint = Point;
		NET_LOG(LogTemp, Warning, TEXT("ServerOnly_FindInteractionPoint"));
		ClientRPC_ChangeNearPoint(NearPoint);
	}
}

void ACamper::ClientRPC_ChangeNearPoint_Implementation(class UInteractionPoint* NewPoint)
{
	FText Description = FText::FromString("NONE");

	if (NewPoint)
	{
		const auto* PointOwner = NewPoint->GetOwner();
		if (Cast<AGenerator>(PointOwner))
		{
			Description = FText::FromString("REPAIR");
		}
		else if (Cast<AMeatHook>(PointOwner))
		{
			Description = FText::FromString("UNHOOK");
		}
		else if (auto* Pallet = Cast<APallet>(PointOwner))
		{
			if (Pallet->bIsFallOnGround)
			{
				Description = FText::FromString("VAULT");
			}
			else
			{
				Description = FText::FromString("PULL DOWN");
			}
		}
		else if (Cast<ACamper>(PointOwner))
		{
			Description = FText::FromString("HEAL");
		}
	}
	
	NearPoint = NewPoint;
	NET_LOG(LogTemp, Warning, TEXT("ClientRPC_ChangeNearPoint : %s"), *Description.ToString());
	if (const auto* SacrificeController = Cast<ASacrificePlayerController>(GetController()))
	{
		if (SacrificeController->Hud)
		{
			SacrificeController->Hud->OnUpdatedNearPoint(NewPoint, Description);
		}
	}
}

void ACamper::TryInteraction()
{
	ServerRPC_TryInteraction();
}

void ACamper::ServerRPC_TryInteraction_Implementation()
{
	if (nullptr == NearPoint)
	{
		return;
	}

	InteractingPoint = NearPoint;
	NearPoint->Interaction(this);
}

void ACamper::CheckInteractPoint()
{
	ServerRPC_CheckInteractPoint();
}

void ACamper::ServerRPC_CheckInteractPoint_Implementation()
{
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
				if (Anim == nullptr || camperFSMComp->curInteractionState == ECamperInteraction::ECI_Repair) return;
				UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *HitResult.GetComponent()->GetName(),camperFSMComp->curInteractionState == ECamperInteraction::ECI_Repair);
				if(interact->bCanInteract)
				{
					interact->Interaction(this);
					InteractingPoint = interact;
					break;
				}
			}
		}
	}
	else
	{
		bFindPoints = false;
	}
}

void ACamper::MultiCastRPC_StartRepair_Implementation()
{
	if (Anim == nullptr || camperFSMComp->curInteractionState == ECamperInteraction::ECI_Repair)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camper : StartRepair : Anim : nullptr"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("발전기 수리 시작"));
	// Interaction 상태 전환
	camperFSMComp->curInteractionState = ECamperInteraction::ECI_Repair;
	SetInteractionState(ECamperInteraction::ECI_Repair);
	// 움직임 멈추기
	GetCharacterMovement()->StopMovementImmediately();
	
	// 시작 애니메이션 몽타주 실행
	Anim->ServerRPC_PlayRepairAnimation(TEXT("GenIn"));
}

void ACamper::MultiCastRPC_EndRepair_Implementation()
{
	if (Anim == nullptr || camperFSMComp->curInteractionState != ECamperInteraction::ECI_Repair)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camper : EndRepair : Anim : nullptr"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("발전기 수리 중단/종료"));

	// InterAction 상태 None으로 전환
	SetInteractionState(ECamperInteraction::ECI_NONE);
	
	// 다시 애니메이션 idle로 바꾸고 wsad 움직일 수 있게 변경
	Anim->ServerRPC_PlayRepairAnimation(TEXT("GenOut"));
}

void ACamper::FailRepair(FName sectionName)
{
	if (Anim == nullptr) return;

	// 발전기 가동은 된 상태로 게이지만 줄어들고 다시 고쳐야 한다
	Anim->ServerRPC_PlayRepairAnimation(sectionName);
}
// Repair End

void ACamper::StopInteract()
{
	ServerRPC_StopInteract();
}

void ACamper::ServerRPC_StopInteract_Implementation()
{
	if (InteractingPoint)
	{
		InteractingPoint->StopInteraction(this);
		InteractingPoint = nullptr;
	}
}

void ACamper::GetDamage(FString weapon)
{
	ServerRPC_GetDamage(weapon);
}

void ACamper::ServerRPC_GetDamage_Implementation(const FString& weapon)
{
	MultiCastRPC_GetDamage(weapon);
}

void ACamper::MultiCastRPC_GetDamage_Implementation(const FString& weapon)
{
	if (camperFSMComp == nullptr)
	{
		NET_LOG(LogTemp, Warning, TEXT("Camper : GetDamage (%s) !!CamperFSM is nullptr"), *weapon);
		return;
	}
	if (camperFSMComp->curInteractionState == ECamperInteraction::ECI_DeadHard)
	{
		ChangeSpeed();
		return;
	}

	// 맞을 때 비명 지르는 부분
	if (injuredScreamCue) PlayScreamSound();
	
	if (weapon == TEXT("Chainsaw"))
	{
		curHP -= 2;
	}
	else
	{
		curHP--;
	}
	NET_LOG(LogTemp, Warning, TEXT("Camper : GetDamage (%s), CurrentHP=%f"), *weapon, curHP);

	if (curHP <= 0)
	{
		if (HasAuthority())
		{
			SetHealthState(ECamperHealth::ECH_Injury);
		}
		if (HasAuthority()) Crawling();
	}
	else
	{
		ChangeSpeed();
	}
}

void ACamper::ChangeSpeed()
{
	// 다친 상태로 전환 (서버와 로컬 모두 실행)
	if (HasAuthority())
	{
		SetHealthState(ECamperHealth::ECH_Injury);
	}

	// 이전 속도를 저장
	beforeSpeed = moveComp->MaxWalkSpeed;

	// 2초 동안 스피드 증가
	moveComp->MaxWalkSpeed *= 2;

	// 2초 후 원래 속도로 복귀
	GetWorld()->GetTimerManager().SetTimer(hitTimerHandle, [this]()
	{
		moveComp->MaxWalkSpeed = beforeSpeed;
		bPlayInjureSound = true;
		PlayInjureSound();
	}, 2.0f, false);
}

void ACamper::Crawling()
{
	CrawlPoint->bCanInteract = true;
	SetStanceState(ECamperStanceState::ECSS_Crawl);
	SetMovementState(ECamperMoveState::ECS_NONE);
}

void ACamper::StartUnLock()
{
	SetInteractionState(ECamperInteraction::ECI_UnLock);
	MultiCastRPC_StartUnLock();
}

void ACamper::MultiCastRPC_StartUnLock_Implementation()
{
	if (Anim == nullptr) return;
	Anim->ServerRPC_PlayUnLockAnimation(TEXT("StartUnLock"));
}

void ACamper::EndUnLock()
{
	SetInteractionState(ECamperInteraction::ECI_NONE);
	MultiCastRPC_EndUnLock();
}

void ACamper::MultiCastRPC_EndUnLock_Implementation()
{
	if (Anim == nullptr) return;
	Anim->ServerRPC_PlayUnLockAnimation(TEXT("OpenDoor"));
}

void ACamper::Hooking(FName sectionName)
{
	MultiCastRPC_Hooking(sectionName);
}

void ACamper::MultiCastRPC_Hooking_Implementation(FName sectionName)
{
	if (Anim == nullptr) return;
	
	if (bFirst == false && camperFSMComp->curInteractionState == ECamperInteraction::ECI_Hook) // State가 Hook라면
	{
		bFirst = true;
		// Injure 사운드를 끄고
		GetWorldTimerManager().ClearTimer(injureTimerHandle);
		
		// 후크 걸릴 때 비명 한 번 재생
		FTimerHandle hookInTimerHandle;
		GetWorldTimerManager().SetTimer(hookInTimerHandle, [this]()
		{
			PlayHookInSound(); // 후크에 걸릴 때 소리 지르는 부분
		}, 0.9f, false);
	}
	else if (bFirst && camperFSMComp->curInteractionState == ECamperInteraction::ECI_NONE) // State가 NONE이라면
	{
		bFirst = false;
		// 다시 InjureSound를 재생
		PlayInjureSound();
		
	}
	
	Anim->ServerRPC_PlayHookingAnimation(sectionName);
}

void ACamper::RescueHooking(FName sectionName)
{
	MultiCastRPC_RescueHooking(sectionName);
}

void ACamper::MultiCastRPC_RescueHooking_Implementation(FName sectionName)
{
	if (Anim == nullptr) return;
	
	Anim->ServerRPC_PlayRescueHookingAnimation(sectionName);
}

void ACamper::CheckRescueTime(ACamper* camper)
{
	// 갈고리 구출 테스트
	testRescueTime += GetWorld()->GetDeltaSeconds();
	UE_LOG(LogTemp, Warning, TEXT("%f"), testRescueTime);
	if (testRescueTime > 1.12f)
	{
		camper->camperFSMComp->curHealthState = ECamperHealth::ECH_Injury;
		camper->SetInteractionState(ECamperInteraction::ECI_NONE);
		camper->RescueHooking(TEXT("HookRescueEnd"));
		testRescueTime = 0.0f;
	}
}

// 살인마가 드는 함수
void ACamper::PickUp()
{
	SetInteractionState(ECamperInteraction::ECI_Carry);
	MultiCastRPC_PickUp();
}

void ACamper::MultiCastRPC_PickUp_Implementation()
{
	if (Anim == nullptr) return;
	Anim->ServerRPC_PickUpAnimation(TEXT("PickUpStart"));
}


// 살인마가 떨어트리는 함수
void ACamper::PickUpDrop()
{
	SetStanceState(ECamperStanceState::ECSS_Idle);
	MultiCastRPC_PickUpDrop();
	
}
void ACamper::MultiCastRPC_PickUpDrop_Implementation()
{
	if (Anim == nullptr) return;
	Anim->ServerRPC_PickUpAnimation(TEXT("PickUpDrop"));
}

void ACamper::SetInteractionState(ECamperInteraction NewState)
{
	ServerRPC_SetInteractionState(NewState);
}

void ACamper::ServerRPC_SetInteractionState_Implementation(ECamperInteraction NewState)
{
	MultiCastRPC_SetInteractionState(NewState);

	if (userState == nullptr)
    {
    	UE_LOG(LogTemp, Warning, TEXT("userState Not Set"));
    	return;
    }

    FUserState NewUserState = userState->UserState;
    NewUserState.Interaction = NewState;
    userState->UserState = NewUserState;
}

void ACamper::MultiCastRPC_SetInteractionState_Implementation(ECamperInteraction NewState)
{
	camperFSMComp->curInteractionState = NewState;
}

void ACamper::SetHealthState(ECamperHealth NewState)
{
	ServerRPC_SetHealthState(NewState);
}

void ACamper::ServerRPC_SetHealthState_Implementation(ECamperHealth NewState)
{
	MultiCastRPC_SetHealthState(NewState);
	
	if (userState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("userState Not Set"));
		return;
	}

	NET_LOG(LogTemp, Warning, TEXT("MultiCastRPC_SetHealthState_Implementation"));
	FUserState NewUserState = userState->UserState;
	NewUserState.Health = NewState;
	userState->UserState = NewUserState;

	if (NewState == ECamperHealth::ECH_Dead)
	{
		ASacrificeGameState* GameState = Cast<ASacrificeGameState>(GetWorld()->GetGameState());
		if (GameState)
		{
			GameState->ServerOnly_OnCamperExitOrDie();
		}
	}
}

void ACamper::MultiCastRPC_SetHealthState_Implementation(ECamperHealth NewState)
{
	camperFSMComp->curHealthState = NewState;

	switch (NewState)
	{
		case ECamperHealth::ECH_Healthy:
			// Healthy 상태가 되면 Injury 사운드 끄기  
			GetWorldTimerManager().ClearTimer(injureTimerHandle);
			break;
		case ECamperHealth::ECH_Injury:
			break;
		case ECamperHealth::ECH_Dead:
			// 죽는 사운드 재생
			PlayDieSound();
	}
}


// 누웠을 때 힐링 및 상대 치료 하는 함수 RPC
void ACamper::StartHealing()
{
	ServerRPC_StartHealing();
}
void ACamper::ServerRPC_StartHealing_Implementation()
{
	MultiCastRPC_StartHealing();
}
void ACamper::MultiCastRPC_StartHealing_Implementation()
{
	if (camperFSMComp->curInteractionState == ECamperInteraction::ECI_Repair
		|| camperFSMComp->curInteractionState == ECamperInteraction::ECI_UnLock
		|| camperFSMComp->curInteractionState == ECamperInteraction::ECI_Healing
					) return;
	
	// 상태 전환
	camperFSMComp->curInteractionState = ECamperInteraction::ECI_Healing;
	camperFSMComp->curMoveState = ECamperMoveState::ECS_NONE;
}
// Stop Healing
void ACamper::StopHealing()
{
	ServerRPC_StopHealing();
}

void ACamper::ServerRPC_StopHealing_Implementation()
{
	MultiCastRPC_StopHealing();
}

void ACamper::MultiCastRPC_StopHealing_Implementation()
{
	if (camperFSMComp->curInteractionState != ECamperInteraction::ECI_Healing) return;
	
	// 생존자의 상호작용 상태를 NONE으로 변경
	camperFSMComp->curInteractionState = ECamperInteraction::ECI_NONE;
}
// Healing Time Check
void ACamper::HealingTimingCheck(float deltaTime)
{
	ServerRPC_HealingTimingCheck(deltaTime);
}
void ACamper::ServerRPC_HealingTimingCheck_Implementation(float deltaTime)
{
	MultiCastRPC_HealingTimingCheck(deltaTime);
}
void ACamper::MultiCastRPC_HealingTimingCheck_Implementation(float deltaTime)
{
	// 치유를 사용할 때 자가치유중이고 발전기 고치고 있거나 문 여는 중이면 return
	if (camperFSMComp->curInteractionState == ECamperInteraction::ECI_Healing &&
		camperFSMComp->curInteractionState != ECamperInteraction::ECI_Repair &&
		camperFSMComp->curInteractionState != ECamperInteraction::ECI_UnLock &&
		camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crawl)
	{
		// 현재치유시간이 치유 시간을 넘어가면 힐링 시간을 초기화하고 HP를 풀로 채우며 다친 상태를 회복시키고 상호작용을 NONE으로 초기화
		if (curhealingTime >= healingTime)
		{
			curhealingTime = 0;
			curHP = 2;
			camperFSMComp->curStanceState = ECamperStanceState::ECSS_Idle;
			camperFSMComp->curHealthState = ECamperHealth::ECH_Healthy;
			camperFSMComp->curInteractionState = ECamperInteraction::ECI_NONE;
			CrawlPoint->bCanInteract = false;
		}
		// 32초동안 치유 해야함.
		curhealingTime += deltaTime;
		UE_LOG(LogTemp, Warning, TEXT("curhealingTime : %f"), curhealingTime);
	}
}
void ACamper::PullDownPallet()
{
	ServerRPC_PullDownPallet();
}

void ACamper::ServerRPC_PullDownPallet_Implementation()
{
	MultiCastRPC_PullDownPallet();
}

void ACamper::MultiCastRPC_PullDownPallet_Implementation()
{
	if (Anim == nullptr) return;
	Anim->ServerRPC_PullDownPalletAnimation(TEXT("PullDownPallet"));
}

// Sound 부분
void ACamper::PlayLeftSound()
{
	if (leftFootCue)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			leftFootCue,
			GetActorLocation(),
			1.0f,
			1.0f,
			0.0f,
			leftFootAttenuation
		);
	}
}

void ACamper::PlayRightSound()
{
	if (rightFootCue)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			rightFootCue,
			GetActorLocation(),
			1.0f,
			1.0f,
			0.0f,
			rightFootAttenuation
		);
	}
}

void ACamper::PlayInjureSound()
{
	if (!injureCue || !injureAttenuation) return;
	
	// 맞은 후 지속적으로 신음소리 내는 부분
	GetWorldTimerManager().SetTimer(injureTimerHandle, [this]()
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			injureCue,
			GetActorLocation(),
			1.0f,
			1.0f,
			0.0f,
			injureAttenuation
		);
	}, 1.0f, true);
}

void ACamper::PlayScreamSound()
{
	if (injuredScreamCue)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			injuredScreamCue,
			GetActorLocation(),
			1.5f,
			1.0f,
			0.0f,
			injuredScreamAttenuation
		);
	}
}

void ACamper::PlayHookInSound()
{
	if (hookInCue)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			hookInCue,
			GetActorLocation(),
			1.5f,
			1.0f,
			0.0f,
			hookInAttenuation
		);
	}
}

void ACamper::PlayDieSound()
{
	if (dieCue)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			dieCue,
			GetActorLocation(),
			1.5f,
			1.0f,
			0.0f
		);
	}
}

void ACamper::PrintNetLog()
{
	FString conStr = GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection");
	FString ownerStr = GetOwner() != nullptr ? GetOwner()->GetActorNameOrLabel() : TEXT("No Owner");
	FString mineStr = IsLocallyControlled() ? TEXT("내 것") : TEXT("남의 것"); // 컨트롤러가 있냐 없냐, 내거냐 아니냐를 판단하는 함수

	// FString stateStr = UEnum::GetValueAsString(curHealthState);
	// GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, *stateStr);
	if (camperFSMComp)
	{
		FString stanceStateStr = UEnum::GetValueAsString(camperFSMComp->curStanceState);
		FString moveStateStr = UEnum::GetValueAsString(camperFSMComp->curMoveState);
		FString healthStateStr = UEnum::GetValueAsString(camperFSMComp->curHealthState);
		FString InteractionStateStr = UEnum::GetValueAsString(camperFSMComp->curInteractionState);
	
		FString logStr = FString::Printf(TEXT("Connection : %s \r\nOwner : %s\r\nMine : %s\r\nStanceState : %s\r\nMoveState : %s\r\nHealthState : %s\r\nInteractionState : %s"), *conStr, *ownerStr, *mineStr, *stanceStateStr, *moveStateStr, *healthStateStr, *InteractionStateStr);
	
		DrawDebugString(GetWorld(), GetActorLocation(), logStr, nullptr, FColor::Red, 0, true);
	}
}

void ACamper::ServerRPC_HealthCheck_Implementation()
{
	if (IsLocallyControlled()) MultiCastRPC_HealthCheck();
}

void ACamper::MultiCastRPC_HealthCheck_Implementation()
{
	if (camperFSMComp) camperFSMComp->curHealthState = ECamperHealth::ECH_Healthy;
	moveComp->MaxWalkSpeed = 0;
	curHP = maxHP;
}

void ACamper::OnInteraction(class UInteractionPoint* Point, AActor* OtherActor) // 쓰러진 생존자 기준으로 OtherActor는 서있는 생존자
{
	UE_LOG(LogTemp, Warning, TEXT("1"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetActorNameOrLabel());
	if (auto* Slasher = Cast<ACanival>(OtherActor))
	{
		Slasher->MulticastRPC_AttachSurvivorToShoulder(this);
		Slasher->InteractingPoint = nullptr;
		Slasher->NearPoint = nullptr;
		Slasher->ClientRPC_ChangeNearPoint(nullptr);
	}
	else if (auto* camper = Cast<ACamper>(OtherActor))
	{
		// FString s = UEnum::GetValueAsString(camperFSMComp->curHealthState);
		// UE_LOG(LogTemp, Warning, TEXT("This Player : %s, Other Player : %s, camper->Anim의 오너 : %s, curHealthState : %s"),
			// *GetActorNameOrLabel(), *OtherActor->GetActorNameOrLabel(), *camper->Anim->TryGetPawnOwner()->GetActorNameOrLabel(), *s);
		
		// 다친 상태일 때 상대 힐하는 몽타주 실행
		UE_LOG(LogTemp, Warning, TEXT("3"));
 		// 자신한테는 치료하라고 함수 실행
 		StartHealing();
 		ServerRPC_StartHealingAnimation(camper);
	}
}
void ACamper::ServerRPC_StartHealingAnimation_Implementation(ACamper* camper)
{
	MultiCastRPC_StartHealingAnimation(camper);
}

void ACamper::MultiCastRPC_StartHealingAnimation_Implementation(ACamper* camper)
{
	// 상대방 애니메이션 몽타주 켜주기
	camper->Anim->ServerRPC_HealingAnimation(TEXT("StartHealing"));
}

void ACamper::OnStopInteraction(class UInteractionPoint* Point, AActor* OtherActor)
{
	// TODO: 생존자 치료
	if (auto* camper = Cast<ACamper>(OtherActor))
	{
		if (camper->Anim)
		{
			// 자신의 치료를 멈추고
			StopHealing();
			ServerRPC_EndHealingAnimation(camper);
		}
	}
}
void ACamper::ServerRPC_EndHealingAnimation_Implementation(ACamper* camper)
{
	MultiCastRPC_EndHealingAnimation(camper);
}
void ACamper::MultiCastRPC_EndHealingAnimation_Implementation(ACamper* camper)
{
	// 상대방 애니메이션도 멈추기
	camper->Anim->ServerRPC_HealingAnimation(TEXT("EndHealing"));
}


void ACamper::OnRescued()
{
	if (HasAuthority())
	{
		MulticastRPC_OnRescued();
	}
}

void ACamper::MulticastRPC_OnRescued_Implementation()
{
	if (camperFSMComp && HasAuthority())
	{
		SetHealthState(ECamperHealth::ECH_Healthy);
		SetMovementState(ECamperMoveState::ECS_NONE);
		SetStanceState(ECamperStanceState::ECSS_Idle);
		SetInteractionState(ECamperInteraction::ECI_NONE);
	}
	curHP = maxHP;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -210), FRotator(0, -90, 0));
    SetActorEnableCollision(true);
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    GetCharacterMovement()->StopMovementImmediately();
}
