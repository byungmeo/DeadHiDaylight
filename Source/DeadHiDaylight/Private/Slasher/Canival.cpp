// Fill out your copyright notice in the Description page of Project Settings.


#include "Canival.h"
#include "CamperAnimInstance.h"


#include "CanivalAnim.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "SacrificeCommonHUD.h"
#include "SacrificePlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Camper.h"

// Sets default values
ACanival::ACanival()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	Hammer->SetRelativeLocation(FVector(17.197212, -14.791651, 20.419429));
	Hammer->SetRelativeRotation(FRotator(85.793405, 6.474225, 59.887648));
	Hammer->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
	Hammer->SetCollisionProfileName(TEXT("OverlapAllDynamic"));



	ChainSaw = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChainSaw"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> ChainSawObj(TEXT("/Script/Engine.SkeletalMesh'/Game/KHA/Carnival/Weapon/ChainSaw/ChainSaw.ChainSaw'"));
	if (ChainSawObj.Succeeded())
	{
		ChainSaw->SetSkeletalMesh(ChainSawObj.Object);
	}
	ChainSaw->SetupAttachment(GetMesh(), TEXT("joint_Hand_LT_01_IK"));
	ChainSaw->SetRelativeLocation(FVector(3.396842,-41.498392,-5.774845));
	ChainSaw->SetRelativeRotation(FRotator(-66.141346,18.747237,143.994784));
	ChainSaw->SetRelativeScale3D(FVector(0.7f,0.7f,0.7f));
	ChainSaw->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
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
}



// Called every frame
void ACanival::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		FindPoint();
		CheckAndAttachSurvivor();
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
		pi->BindAction(ia_Kick, ETriggerEvent::Started,this, &ACanival::FindPoint);
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

void ACanival::Look(const FInputActionValue& InputActionValue)
{
	FVector2D Value = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
	
}

void ACanival::LeftClick_Start()
{
	AnimInstance->PlayHammrInAnimation();
	Hammer->SetGenerateOverlapEvents(true);
}

void ACanival::LeftClick_Complet()
{
	
	AnimInstance->PlayHammerSwingAnimation();
	Hammer->SetGenerateOverlapEvents(true);
}

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
}

void ACanival::RightAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("ACanival::RightAttack"));
	bIsAttacking=true;
	ChainSaw->SetGenerateOverlapEvents(true);
	AnimInstance->PlayChainSawRunAnimation();
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
			if (!(Camper->Anim && Camper->camperFSMComp && Camper->camperFSMComp->curStanceState == ECamperStanceState::ECSS_Crawl))
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
	
	AnimInstance->PlayAttackShoulderAnimation();
	//어깨 부착
	if (Survivor && Survivor->GetMesh())
	{
		// AActor* ParentActor, FName SocketName, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies
		// Survivor->K2_AttachToActor(this, TEXT("joint_ShoulderLT_01Socket"), );
		Survivor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("joint_ShoulderLT_01Socket"));
		Survivor->GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));

		Survivor->SetActorEnableCollision(false);
		Survivor->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		Survivor->GetCharacterMovement()->StopMovementImmediately();
		Survivor->GetMesh()->bPauseAnims = true;
		UE_LOG(LogTemp, Warning, TEXT("어깨에 붙음"));
	}
	
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
}

void ACanival::HangOnHook(class AMeatHook* Hook)
{
	AnimInstance->PlayHangAnimation(Hook);
	UE_LOG(LogTemp, Warning, TEXT("Hang"));
}


void ACanival::Kick()
{
	AnimInstance->PlayKickAnimation();
	UE_LOG(LogTemp, Warning, TEXT("Kick"));
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
		UE_LOG(LogTemp, Warning, TEXT("ACanival::OnHammerBeginOverlap"));
		Hammer->SetGenerateOverlapEvents(false);
		// Camper->야 너 맞았어
		AnimInstance->PlayWipeAnimation();
		Camper->GetDamage(TEXT(""));
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
		Camper->GetDamage(TEXT("Chainsaw"));
		
	}
	
	// 벽이냐
	// 그 외냐
}


void ACanival::FindPoint()
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
		EDrawDebugTrace::ForDuration,
		OutHits,
		true
	);
	if (bHit)
	{
		for (const auto HitResult : OutHits)
		{
			
			if (auto interact = Cast<UInteractionPoint>(HitResult.GetComponent()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Find InteractionPoint"));
				interact->Interaction(this);
				break;
			}
			
		}
	}
}
