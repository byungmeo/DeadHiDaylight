// Fill out your copyright notice in the Description page of Project Settings.


#include "Canival.h"
#include "CamperAnimInstance.h"


#include "CanivalAnim.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Generator.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InteractionPoint.h"
#include "MeatHook.h"
#include "Pallet.h"
#include "SacrificeCommonHUD.h"
#include "SacrificePlayerController.h"
#include "Camera/CameraComponent.h"
#include "CamperComps/CamperFSM.h"
#include "DeadHiDaylight/DeadHiDaylight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Camper.h"
#include "Sound/SoundBase.h"
#include "Net/UnrealNetwork.h"



// Sets default values
ACanival::ACanival()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KHA/Carnival/Character/Carnival.Carnival'"));
	if (MeshObj.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshObj.Object);
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
	
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/KHA/Carnival/Inputs/IMC_Canival.IMC_Canival'"));
	if (TempIMC.Succeeded())
	{
		imc_carnival = TempIMC.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("/Script/EnhancedInput.InputAction'/Game/KHA/Carnival/Inputs/IA_CanivalMove.IA_CanivalMove'"));
	if (TempIAMove.Succeeded())
	{
		ia_move = TempIAMove.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIALook(TEXT("/Script/EnhancedInput.InputAction'/Game/KHA/Carnival/Inputs/IA_CanivalLook.IA_CanivalLook'"));
	if (TempIALook.Succeeded())
	{
		ia_look = TempIALook.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIALeftClick(TEXT("/Script/EnhancedInput.InputAction'/Game/KHA/Carnival/Inputs/IA_CanivalLeftClick.IA_CanivalLeftClick'"));
	if (TempIALeftClick.Succeeded())
	{
		ia_leftClick = TempIALeftClick.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempIARightClick(TEXT("/Script/EnhancedInput.InputAction'/Game/KHA/Carnival/Inputs/IA_CanivalRightClick.IA_CanivalRightClick'"));
	if (TempIARightClick.Succeeded())
	{
		ia_rightClick = TempIARightClick.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAKick(TEXT("/Script/EnhancedInput.InputAction'/Game/KHA/Carnival/Inputs/IA_CanivalKick.IA_CanivalKick'"));
	if (TempIAKick.Succeeded())
	{
		ia_Kick = TempIAKick.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAHang(TEXT("/Script/EnhancedInput.InputAction'/Game/KHA/Carnival/Inputs/IA_Hang.IA_Hang'"));
	if (TempIAHang.Succeeded())
	{
		ia_hang = TempIAHang.Object;
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KHA/Carnival/Character/ABP_Canival.ABP_Canival_C'"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh(), TEXT("joint_Cam_01"));
	Camera->bUsePawnControlRotation = true;

	Hammer = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hammer"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> HammerObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KHA/Carnival/Weapon/Hammer/Hammer.Hammer'"));
	if (HammerObj.Succeeded())
	{
		Hammer->SetSkeletalMesh(HammerObj.Object);
	}
	Hammer->SetupAttachment(GetMesh(), TEXT("joint_Hand_RT_01_IK"));
	Hammer->SetRelativeLocation(FVector(1.114717,-4.765961,15.095930));
	Hammer->SetRelativeRotation(FRotator(6.474225,59.887648,105.793405));
	Hammer->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	Hammer->SetCollisionProfileName(TEXT("OverlapAllDynamic"));



	ChainSaw = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChainSaw"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> ChainSawObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KHA/Carnival/Weapon/ChainSaw/ChainSaw.ChainSaw'"));
	if (ChainSawObj.Succeeded())
	{
		ChainSaw->SetSkeletalMesh(ChainSawObj.Object);
	}
	ChainSaw->SetupAttachment(GetMesh(), TEXT("joint_Hand_LT_01_IK"));
	ChainSaw->SetRelativeLocation(FVector(-3.069078,-40.038100,2.546956));
	ChainSaw->SetRelativeRotation(FRotator(4.950710,137.963032,-67.389997));
	ChainSaw->SetRelativeScale3D(FVector(0.5f,0.5f,0.5f));
	ChainSaw->SetCollisionProfileName(TEXT("OverlapAllDynamic"));


	ConstructorHelpers::FObjectFinder<USoundBase> SoundObj(TEXT("/Script/Engine.SoundWave'/Game/KHA/Carnival/Character/Audio/K09_WEAPON/AudioEvent_K09_WPN_Default_SledgeHammer_Impact_Survivor__r_.AudioEvent_K09_WPN_Default_SledgeHammer_Impact_Survivor__r_'"));
	if (SoundObj.Succeeded())
	{
		HammerHitSound = SoundObj.Object;
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

	AnimInstance = Cast<UCanivalAnim>(GetMesh()->GetAnimInstance());
	Hammer->OnComponentBeginOverlap.AddDynamic(this, &ACanival::OnHammerBeginOverlap);
	ChainSaw->OnComponentBeginOverlap.AddDynamic(this, &ACanival::OnChainSawBeginOverlap);
	

	//전기톱 UI
	// FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	// UE_LOG(LogTemp, Warning, TEXT("현재 레벨에서 UI나옴1"));
	// if (CurrentLevelName.Equals("KHATestMap"))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("현재 레벨에서 UI나옴2"));
	// 	if (WidgetClass)
	// 	{
	// 		
	// 		ChainsawWidgetInstance = CreateWidget<UCanivalUI>(GetWorld(), WidgetClass);
	// 		if (ChainsawWidgetInstance)
	// 		{
	// 			ChainsawWidgetInstance->AddToViewport();
	// 		}
	// 	}
	// }

	if (const auto* SacrificeController = Cast<ASacrificePlayerController>(Controller))
	{
		CommonHud = SacrificeController->Hud;
	}
	
	if (HasAuthority())
	{
		FTimerHandle FindPointTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FindPointTimerHandle, this, &ACanival::ServerOnly_FindInteractionPoint, 0.1f, true);
	}
}



// Called every frame
void ACanival::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		ServerRPC_SendViewRotation(Camera->GetRelativeRotation());
	}
	
	if (bChainSawCharging || ChainSawGauge > 0)
	{
		if (bChainSawCharging)
		{
			ChainSawGauge += DeltaTime * ChainSawIncPerSec;
		}
		else
		{
			ChainSawGauge -= DeltaTime * ChainSawIncPerSec;
		}

		if (CommonHud)
		{
			if (ChainSawGauge >= 1 || ChainSawGauge < 0)
			{
				if (ChainSawGauge >= 1)
				{
					RightAttack();
				}
				bChainSawCharging = false;
				ChainSawGauge = 0;
				CommonHud->OnHiddenGaugeBar();
				GetCharacterMovement()->MaxWalkSpeed = InitSpeed;
			}
			else
			{
				CommonHud->OnUpdateGaugeBar(ChainSawGauge);
			}
		}
	}
}

void ACanival::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pi = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (pi)
	{
		pi->BindAction(ia_move, ETriggerEvent::Triggered,this, &ACanival::Move);
		pi->BindAction(ia_look, ETriggerEvent::Triggered,this, &ACanival::Look);
		pi->BindAction(ia_leftClick, ETriggerEvent::Started,this, &ACanival::LeftClick_Start);
		pi->BindAction(ia_leftClick, ETriggerEvent::Completed ,this, &ACanival::LeftClick_Complet);
		pi->BindAction(ia_rightClick, ETriggerEvent::Started,this, &ACanival::RightClick_Start);
		pi->BindAction(ia_rightClick, ETriggerEvent::Completed ,this, &ACanival::RightClick_Complet);
		pi->BindAction(ia_Kick, ETriggerEvent::Started,this, &ACanival::TryInteraction);
		// pi->BindAction(ia_hang, ETriggerEvent::Started,this, &ACanival::HangOnHook);
	}
}

void ACanival::Move(const struct FInputActionValue& inputValue)
{
	// input is a Vector2D
	FVector2D MovementVector = inputValue.Get<FVector2D>();

	// add movement 
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ACanival::OnRep_ViewRotation()
{
	if (false == IsLocallyControlled())
	{
		Camera->SetRelativeRotation(FRotator(ViewRotation.Pitch, ViewRotation.Yaw, 0.f));
	}
}

void ACanival::Look(const FInputActionValue& InputActionValue)
{
	FVector2D Value = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ACanival::ServerRPC_SendViewRotation_Implementation(FRotator NewRotation)
{
	ViewRotation = NewRotation;
	Camera->SetRelativeRotation(FRotator(ViewRotation.Pitch, ViewRotation.Yaw, 0.f));
}

void ACanival::SetMovementState(ECanivalMoveState NewState)
{
	if (CurrentMoveState == NewState)
		return;

	if (HasAuthority())
	{
		// 서버인 경우 바로 멀티캐스트 호출
		MultiCast_SetMovementState(NewState);
	}
	else
	{
		// 클라이언트인 경우 서버에 요청
		Server_SetMovementState(NewState);
	}
}

// 서버 RPC 구현: 클라이언트 요청을 받아 모든 클라이언트에 전파
void ACanival::Server_SetMovementState_Implementation(ECanivalMoveState NewState)
{
	MultiCast_SetMovementState(NewState);
}

bool ACanival::Server_SetMovementState_Validate(ECanivalMoveState NewState)
{
	return true;
}

// 멀티캐스트 RPC 구현: 모든 클라이언트에서 이동 상태 및 관련 속도를 업데이트
void ACanival::MultiCast_SetMovementState_Implementation(ECanivalMoveState NewState)
{
	CurrentMoveState = NewState;
	// 예시: 이동 상태에 따라 이동 속도 업데이트 (InitSpeed는 기본 속도)
	switch (NewState)
	{
	case ECanivalMoveState::CMS_Idle:
		GetCharacterMovement()->MaxWalkSpeed = InitSpeed;
		break;
	case ECanivalMoveState::CMS_Move:
		GetCharacterMovement()->MaxWalkSpeed = InitSpeed * 1.5f; // 예시 값
		break;
	case ECanivalMoveState::CMS_Run:
		GetCharacterMovement()->MaxWalkSpeed = InitSpeed * 2.0f; // 예시 값
		break;
	default:
		break;
	}
}

// Replication 설정
void ACanival::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACanival, CurrentMoveState);
	DOREPLIFETIME(ACanival, ViewRotation);
}

//망치 공격 
void ACanival::LeftClick_Start()
{
	Server_LeftClickStart();
}

void ACanival::Server_LeftClickStart_Implementation()
{
	MultiCast_LeftClickStart();
}

void ACanival::MultiCast_LeftClickStart_Implementation()
{
	if (AnimInstance)
	{
		AnimInstance->PlayHammrInAnimation();
	}
}

void ACanival::LeftClick_Complet()
{
	Server_LeftClickComplete();
}
void ACanival::Server_LeftClickComplete_Implementation()
{
	Hammer->SetGenerateOverlapEvents(true);
	MultiCast_LeftClickComplete();
}

void ACanival::MultiCast_LeftClickComplete_Implementation()
{
	AnimInstance->PlayHammerSwingAnimation();
}

//전기톱 공
void ACanival::RightClick_Start()
{
	GetCharacterMovement()->MaxWalkSpeed *= 0.8f;

	if (ChainSawGauge <= 0)
	{
		AnimInstance->PlayChainSawAttackAnimation();
	}
	// bIsCharging = true;
	bIsAttacking = false;
	
	// GetWorld()->GetTimerManager().SetTimer(RigthAttackTimerHandle,this, &ACanival::RightAttack, RigthAttackDelay,false);

	if (CommonHud)
	{
		CommonHud->OnVisibleGaugeBar(FText::FromName(TEXT("USE CHAINSAW")), 0);
	}
	bChainSawCharging = true;
	
	
	if (HasAuthority())
	{
		MultiCast_RightClickStart();
	}
	else
	{
		Server_RightClickStart();
	}
}

void ACanival::Server_RightClickStart_Implementation()
{
	MultiCast_RightClickStart();
}

bool ACanival::Server_RightClickStart_Validate()
{
	return true;
}
void ACanival::MultiCast_RightClickStart_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed *= 0.8f;
	if (ChainSawGauge <= 0)
	{
		if (AnimInstance)
		{
			AnimInstance->PlayChainSawAttackAnimation();
		}
	}
	bIsAttacking = false;
	if (CommonHud)
	{
		CommonHud->OnVisibleGaugeBar(FText::FromName(TEXT("USE CHAINSAW")), 0);
	}
	bChainSawCharging = true;
}


void ACanival::RightClick_Complet()
{
	if (bChainSawCharging)
	{
		bChainSawCharging = false;
	}
	
	//버튼 떼었을 때 타이머가 아직 실행중이면
	if (!bIsAttacking)
	{
		// GetCharacterMovement()->MaxWalkSpeed *= 2;
		// GetWorld()->GetTimerManager().ClearTimer(RigthAttackTimerHandle);//타이머 취소
		//AnimInstance->PlayChainSawRunAnimation(); //아이들상태로 (코드 변경해야함)
		
	}


	if (HasAuthority())
	{
		MultiCast_RightClickComplete();
	}
	else
	{
		Server_RightClickComplete();
	}
}
void ACanival::Server_RightClickComplete_Implementation()
{
	MultiCast_RightClickComplete();
}

bool ACanival::Server_RightClickComplete_Validate()
{
	return true;
}
void ACanival::MultiCast_RightClickComplete_Implementation()
{
	if (bChainSawCharging)
	{
		bChainSawCharging = false;
	}
}


void ACanival::RightAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("ACanival::RightAttack"));
	bIsAttacking=true;
	ChainSaw->SetGenerateOverlapEvents(true);
	AnimInstance->PlayChainSawRunAnimation();

	if (HasAuthority())
	{
		MultiCast_RightAttack();
	}
	else
	{
		Server_RightAttack();
	}
}
void ACanival::Server_RightAttack_Implementation()
{
	MultiCast_RightAttack();
}

bool ACanival::Server_RightAttack_Validate()
{
	return true;
}

void ACanival::MultiCast_RightAttack_Implementation()
{
	bIsAttacking = true;
	if (ChainSaw)
	{
		ChainSaw->SetGenerateOverlapEvents(true);
	}
	if (AnimInstance)
	{
		AnimInstance->PlayChainSawRunAnimation();
	}
}



void ACanival::CheckAndAttachSurvivor()
{
	// 이미 부착된 생존자가 있으면 새로 찾지 않음
	if (AttachedSurvivor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("생존자 이미 붙어있음"));
		return;
	}
    
	// 모든 ACamper 액터를 검색
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACamper::StaticClass(), FoundActors);
    
	float ClosestDistance = FLT_MAX;
	ACamper* NearestCamper = nullptr;
    
	for (AActor* Actor : FoundActors)
	{
		ACamper* Camper = Cast<ACamper>(Actor);
		if (Camper)
		{
			// 생존자가 죽어서 크롤링 상태인지 확인 (Anim 인스턴스가 있고 bCrawl이 true)
			if (!(Camper->camperFSMComp && Camper->camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crawl))
			{
				continue;
			}
            
			float Dist = FVector::Distance(GetActorLocation(), Camper->GetActorLocation());
			if (Dist < ClosestDistance && Dist <= distanceToSurvivor)
			{
				ClosestDistance = Dist;
				NearestCamper = Camper;
			}
		}
	}
    
	if (NearestCamper)
	{
		// 가장 가까운 생존자를 어깨 소켓에 부착
		AttachSurvivorToShoulder(NearestCamper);
		AttachedSurvivor = NearestCamper;
		UE_LOG(LogTemp, Warning, TEXT("Attached survivor %s to shoulder."), *NearestCamper->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No crawl state survivor found within range."));
	}
	
	//죽은 생존자 찾기
	// if (AttachedSurvivor!=nullptr)
	// {
	// 	return;
	// }
	// TArray<AActor*> allSurvivors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACamper::StaticClass(),allSurvivors);
	//
	// float closetDist = std::numeric_limits<float>::max();
	// ACamper* closestCamper = nullptr;

	// for (AActor* Actor : allSurvivors)
	// {
	// 	ACamper* Camper = Cast<ACamper>(Actor);
	// 	if (!Camper)
	// 	{
	// 		continue;
	// 	}
	// 	// 생존자의 애님이 유효하고, bCrawl이 true인지 확인 
	// 	if (!(Camper->Anim && Camper->Anim->bCrawl))
	// 	{
	// 		continue;
	// 	}
	// 	float dist = FVector::Distance(GetActorLocation(), Camper->GetActorLocation());
	// 	// 설정한 범위(distanceToSurvivor) 내에 있는지 확인하고 최단 거리 업데이트
	// 	if (dist <= distanceToSurvivor  && dist < closetDist)
	// 	{
	// 		closetDist = dist;
	// 		closestCamper = Camper;
	// 	}
	// }
 //    
	// if (closestCamper != nullptr)
	// {
	// 	AttachSurvivorToShourder(closestCamper);
	// 	AttachedSurvivor = closestCamper;
	// 	UE_LOG(LogTemp, Warning, TEXT("Attached survivor %s to shoulder."), *closestCamper->GetName());
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("No crawl state survivor found within range."));
	// }
	
}

void ACanival::AttachSurvivorToShoulder(class ACamper* Survivor)
{
	AttachedSurvivor = Survivor;
	
	AnimInstance->PlayAttackShoulderAnimation();
	//어깨 부착
	if (Survivor && Survivor->GetMesh())
	{
		// AActor* ParentActor, FName SocketName, EAttachmentRule LocationR`ule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies
		// Survivor->K2_AttachToActor(this, TEXT("joint_ShoulderLT_01Socket"), );
		Survivor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("joint_ShoulderLT_01Socket"));
		Survivor->GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));

		Survivor->CrawlPoint->bCanInteract = false;
		Survivor->SetActorEnableCollision(false);
		Survivor->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		Survivor->GetCharacterMovement()->StopMovementImmediately();
		// Survivor->GetMesh()->bPauseAnims = true;
		UE_LOG(LogTemp, Warning, TEXT("어깨에 붙음"));
	}
	
}



void ACanival::HangOnHook(class AMeatHook* Hook)
{
	MulticastRPC_HangOnHook(Hook);
}

void ACanival::MulticastRPC_HangOnHook_Implementation(class AMeatHook* Hook)
{
	AnimInstance->PlayHangAnimation(Hook);
}

void ACanival::KickGenerator(class UInteractionPoint* Point)
{
	InteractingPoint = nullptr;
	NearPoint = nullptr;
	MulticastRPC_KickGenerator(Point);
}

void ACanival::MulticastRPC_KickGenerator_Implementation(class UInteractionPoint* Point)
{
	AnimInstance->PlayKickGeneratorAnimation(Point);
}


void ACanival::KickPallet(class UInteractionPoint* Point)
{
	InteractingPoint = nullptr;
	NearPoint = nullptr;
	MulticastRPC_KickPallet(Point);
}

void ACanival::MulticastRPC_KickPallet_Implementation(class UInteractionPoint* Point)
{
	AnimInstance->PlayKickPalletAnimation(Point);
}


void ACanival::OnHammerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	// 생존자냐
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		if (Camper->camperFSMComp)
		{
			if (Camper->camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crawl)
			{
				return;
			}
		}
		NET_LOG(LogTemp, Warning, TEXT("ACanival::OnHammerBeginOverlap"));
		Hammer->SetGenerateOverlapEvents(false);
		Camper->GetDamage("");
		MulticastRPC_OnHammerHit();
	}
	// 벽이냐
	// 그 외냐
}

void ACanival::OnChainSawBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	// 생존자냐
	if (ACamper* Camper = Cast<ACamper>(OtherActor))
	{
		//UE_LOG(LogTemp, Warning, TEXT("ACanival::OnChainSawBeginOverlap"));
		// ChainSaw->SetGenerateOverlapEvents(false);
		// Camper->야 너 맞았어
		if (Camper->camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crawl)
		{
			return;
		}
		Camper->GetDamage("Chainsaw");
	}
	
	// 벽이냐
	// 그 외냐
}

void ACanival::ServerOnly_FindInteractionPoint()
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
	auto* Point = UInteractionPoint::FindInteractionPoint(this, StartEnd, StartEnd, EInteractionMode::EIM_SlasherOnly);
	if (NearPoint != Point)
	{
		NearPoint = Point;
		NET_LOG(LogTemp, Warning, TEXT("ServerOnly_FindInteractionPoint"));
		ClientRPC_ChangeNearPoint(NearPoint);
	}
}

void ACanival::ClientRPC_ChangeNearPoint_Implementation(class UInteractionPoint* NewPoint)
{
	FText Description = FText::FromString("NONE");

	if (NewPoint)
	{
		const auto* PointOwner = NewPoint->GetOwner();
		if (Cast<AGenerator>(PointOwner))
		{
			Description = FText::FromString("BREAK");
		}
		else if (Cast<AMeatHook>(PointOwner))
		{
			Description = FText::FromString("HOOK SURVIVOR");
		}
		else if (Cast<APallet>(PointOwner))
		{
			Description = FText::FromString("BREAK");
		}
		else if (Cast<ACamper>(PointOwner))
		{
			Description = FText::FromString("CARRY SURVIVOR");
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

void ACanival::TryInteraction()
{
	ServerRPC_TryInteraction();
}

void ACanival::ServerRPC_TryInteraction_Implementation()
{
	if (nullptr == NearPoint)
	{
		return;
	}

	InteractingPoint = NearPoint;
	NearPoint->Interaction(this);
}

void ACanival::StopInteract()
{
	ServerRPC_StopInteract();
}

void ACanival::MulticastRPC_OnHammerHit_Implementation()
{
	AnimInstance->PlayWipeAnimation();
	UGameplayStatics::PlaySoundAtLocation(this, HammerHitSound, GetActorLocation());
	NET_LOG(LogTemp, Warning, TEXT("MulticastRPC_OnHammerHit_Implementation"));
}

void ACanival::ServerRPC_StopInteract_Implementation()
{
	if (InteractingPoint)
	{
		InteractingPoint->StopInteraction(this);
		InteractingPoint = nullptr;
	}
}