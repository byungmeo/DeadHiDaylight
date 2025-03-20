// Fill out your copyright notice in the Description page of Project Settings.


#include "Canival.h"

#include "Camper.h"
#include "CanivalAnim.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAKick(TEXT("/Script/EnhancedInput.InputAction'/Game/KHA/Carnival/Inputs/IA_CanivalKick.IA_CanivalKick'"));
	if (TempIAKick.Succeeded())
	{
		ia_Kick = TempIAKick.Object;
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
	Hammer->SetRelativeLocation(FVector(2.060449, 14.845951, 6.233762));
	Hammer->SetRelativeRotation(FRotator(7.372038, 110.272844, 92.284188));
	Hammer->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	Hammer->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
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
}



// Called every frame
void ACanival::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One))
	{
		FindPoint();
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
		pi->BindAction(ia_leftClick, ETriggerEvent::Started,this, &ACanival::LeftClick);
		pi->BindAction(ia_Kick, ETriggerEvent::Started,this, &ACanival::FindPoint);
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

void ACanival::LeftClick()
{
	AnimInstance->PlayLeftClickAnimation();
	Hammer->SetGenerateOverlapEvents(true);
}

void ACanival::Kick()
{
	AnimInstance->PlayKickAnimation();
	UE_LOG(LogTemp, Warning, TEXT("Kick"));
}

void ACanival::CompletedKick()
{
	
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
