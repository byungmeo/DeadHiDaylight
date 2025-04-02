// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Canival.generated.h"
UENUM(BlueprintType)
enum class ECanivalMoveState : uint8
{
	CMS_Idle UMETA(DisplayName="Idle"),
	CMS_Move UMETA(DisplayName="Move"),
	CMS_Run  UMETA(DisplayName="Run")
};


UCLASS()
class DEADHIDAYLIGHT_API ACanival : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACanival();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	void Move(const struct FInputActionValue& inputValue);
	void Look(const FInputActionValue& InputActionValue);
	UFUNCTION(CallInEditor)
	void LeftClick_Start();
	UFUNCTION(CallInEditor)
	void LeftClick_Complet();
	UFUNCTION(CallInEditor)
	void HangOnHook(class AMeatHook* Hook);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(
	class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* Camera = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Hammer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* ChainSaw = nullptr;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* imc_carnival;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_move;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputAction> ia_look;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	const UInputAction* ia_leftClick;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	const UInputAction* ia_rightClick;
	
	UPROPERTY()
	class UCanivalAnim* AnimInstance = nullptr;
	
	TObjectPtr<UInputAction> ia_Kick;
	
	TObjectPtr<UInputAction> ia_hang;

	UFUNCTION()
	void OnHammerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void FindPoint();
	void KickGenerator(class UInteractionPoint* Point);
	void KickPallet(class UInteractionPoint* Point);
	
	
	bool bIsAttacking; //공격 실행되었는지
	bool bIsIdleAndMove; //idle move 상태

	//죽은 생존자 관련 코드
	//생존자 어깨에 붙었는지 안붙었는지 체크
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ACamper* AttachedSurvivor=nullptr;
	
	//범위 체크
	UPROPERTY(VisibleAnywhere)
	float distanceToSurvivor  = 500.0f;
	
	UFUNCTION()
	void CheckAndAttachSurvivor();
	
	//죽은 생존자 어깨에 붙이기
	UFUNCTION()
	void AttachSurvivorToShoulder(class ACamper* Survivor);
	


	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	// TSubclassOf<UCanivalUI> WidgetClass;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	// UCanivalUI* ChainsawWidgetInstance;



	/*
	 * 전기톱 공격 관련
	 */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class USacrificeCommonHUD> CommonHud = nullptr;
	float InitSpeed = 600.0f;
	bool bChainSawCharging = false;
	float ChainSawIncPerSec = 0.33f;
	float ChainSawGauge = 0.0f;

	UFUNCTION(CallInEditor)
	void RightClick_Start();
	UFUNCTION(CallInEditor)
	void RightClick_Complet();
	
	UFUNCTION()
	void RightAttack();
	UFUNCTION()
	void OnChainSawBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HammerHitSound;

	// RPC 관련 함수 (이동)
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	ECanivalMoveState CurrentMoveState;

	void SetMovementState(ECanivalMoveState NewState);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMovementState(ECanivalMoveState NewState);
	virtual void Server_SetMovementState_Implementation(ECanivalMoveState NewState);
	virtual bool Server_SetMovementState_Validate(ECanivalMoveState NewState);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_SetMovementState(ECanivalMoveState NewState);
	virtual void MultiCast_SetMovementState_Implementation(ECanivalMoveState NewState);

	// RPC 관련 함수 (좌클릭)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LeftClickStart();
	virtual void Server_LeftClickStart_Implementation();
	virtual bool Server_LeftClickStart_Validate();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_LeftClickStart();
	virtual void MultiCast_LeftClickStart_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LeftClickComplete();
	virtual void Server_LeftClickComplete_Implementation();
	virtual bool Server_LeftClickComplete_Validate();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_LeftClickComplete();
	virtual void MultiCast_LeftClickComplete_Implementation();

	// RPC 관련 함수 (우클릭)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RightClickStart();
	virtual void Server_RightClickStart_Implementation();
	virtual bool Server_RightClickStart_Validate();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_RightClickStart();
	virtual void MultiCast_RightClickStart_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RightClickComplete();
	virtual void Server_RightClickComplete_Implementation();
	virtual bool Server_RightClickComplete_Validate();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_RightClickComplete();
	virtual void MultiCast_RightClickComplete_Implementation();

	// RPC 관련 함수 (전기톱)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RightAttack();
	virtual void Server_RightAttack_Implementation();
	virtual bool Server_RightAttack_Validate();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_RightAttack();
	virtual void MultiCast_RightAttack_Implementation();








	
	/*
	 *	상호작용 관련
	 */
	UPROPERTY()
	class UInteractionPoint* NearPoint = nullptr;
	void ServerOnly_FindInteractionPoint();
	UFUNCTION(Client, Reliable)
	void ClientRPC_ChangeNearPoint(class UInteractionPoint* NewPoint);
	
	void TryInteraction();
	UFUNCTION(Server, Reliable)
	void ServerRPC_TryInteraction();
	
	void StopInteract();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopInteract();
	
	// 현재 상호작용을 진행 중인 Point
	UPROPERTY()
	class UInteractionPoint* InteractingPoint = nullptr;

	///////////
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_OnHammerHit();
};

