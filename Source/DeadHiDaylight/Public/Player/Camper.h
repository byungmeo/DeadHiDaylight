#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "InteractionPoint.h"
#include "SacrificePlayerState.h"
#include "GameFramework/Character.h"
#include "Camper.generated.h"

UCLASS()
class DEADHIDAYLIGHT_API ACamper : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACamper();

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Camper 매쉬 등 기본 세팅
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* glassesComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* hairComp;
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* cameraComp;

	// Sound
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundCue* leftFootCue;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundAttenuation* leftFootAttenuation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundCue* rightFootCue;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundAttenuation* rightFootAttenuation;

	// Injure Sound
	UPROPERTY()
	class UAudioComponent* injuredAudioComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundCue* injureCue;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundAttenuation* injureAttenuation;
	
	bool bPlayInjureSound = false;
	
	// Injure Scream Sound
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundCue* injuredScreamCue;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundAttenuation* injuredScreamAttenuation;
	
	// 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* deadHardMontage;
	
	// moveComp
	UPROPERTY()
	class UCharacterMovementComponent* moveComp; // 캐릭터 움직임 컴포넌트
	UPROPERTY(EditAnywhere)
	class UCamperAnimInstance* Anim;
	
	// Input 변수
	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Camper; //Input IMC 변수
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Move; 
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Look;
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Run;
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Crouch;
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Repair;
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_UnLock;

	// 타이머 핸들 변수
	FTimerHandle hitTimerHandle;
	
	// 체력 변수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float maxHP = 2;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float curHP = maxHP;
	
	// 속도 변수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float curSpeed = 0; // 현재 속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float moveSpeed = 226; // 걷는 속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float maxSpeed = 400; // 뛰는 속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float crouchSpeed = 113; // 앉아서 걷는 속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float crawlSpeed = 70;
	UPROPERTY(EditAnywhere)
	float beforeSpeed = 0; // 이전 속도 저장용 변수
	
	// 포인트 찾았는지 체크하는 변수
	bool bFindPoints = false;

	// 이동 관련 불 변수
	// bool bIsMoveing = false;
	// bool bIsRuning = false;
	// bool bIsCrouching = false;
	// bool bIsCrawling = false;

	// 이동 관련 함수
	void CamperMove(const FInputActionValue& value); // 캠퍼 움직임 함수
	void StopCamperMove(const FInputActionValue& value);

	void StartRun(const struct FInputActionValue& value); // 캠퍼 뛰는 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartRun();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StartRun();

	void StopRun(const struct FInputActionValue& value);
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopRun();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StopRun();
	
	void Start_Crouch(const struct FInputActionValue& value); // 앉기 시작 함수 
	UFUNCTION(Server, Reliable)
	void ServerRPC_Start_Crouch();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_Start_Crouch();

	void End_Crouch(const struct FInputActionValue& value); // 앉기 끝 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_End_Crouch();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_End_Crouch();
	
	void SetStanceState(ECamperStanceState NewState);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetStanceState(ECamperStanceState NewState);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_SetStanceState(ECamperStanceState NewState);
	
	void SetMovementState(ECamperMoveState NewState);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetMovementState(ECamperMoveState NewState);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_SetMovementState(ECamperMoveState NewState);
	
	// 카메라 관련 함수
	void Look(const struct FInputActionValue& value);  // 카메라 움직임 함수

	/*
	 * Temp for Interact with Generator
	 */
public:
	// 발전기 시작 / 실패
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StartRepair();
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_EndRepair();

	void FailRepair(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_FailRepair(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_FailRepair(FName sectionName);
	// 주변 상호작용 포인트 탐지 함수
	void CheckInteractPoint();
	UFUNCTION(Server, Reliable)
	void ServerRPC_CheckInteractPoint();

	
	void StopInteract();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopInteract();
	UPROPERTY()
	UInteractionPoint* SaveInteract;

public:
	UPROPERTY(VisibleAnywhere, Category = PerksComponent)
	class UPerksComponent* perksComp;
	
	UPROPERTY(VisibleAnywhere, Category = PerksComponent)
	class UCamperFSM* camperFSMComp;
	// 대미지 받았을 때 처리하는 함수 RPC
	void GetDamage(FString weapon);
	UFUNCTION(Server, Reliable)
	void ServerRPC_GetDamage(const FString& weapon);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_GetDamage(const FString& weapon);
	
	// 쓰러진 상태 함수 RPC
	void Crawling();
	UFUNCTION(Server, Reliable)
	void ServerRPC_Crawling();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_Crawling();

	// 문 여는 함수 RPC
	void StartUnLock();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartUnLock();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StartUnLock();
	// 문 닫는 함수 RPC
	void EndUnLock();
	UFUNCTION(Server, Reliable)
	void ServerRPC_EndUnLock();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_EndUnLock();
	// 갈고리 걸리는 함수 RPC
	void Hooking(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_Hooking(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void NetMultiCastRPC_Hooking(FName sectionName);
	
	// 갈고리에서 구해주는 RPC
	void RescueHooking(FName sectionName);
	UFUNCTION(Server, Reliable)
	void ServerRPC_RescueHooking(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void NetMultiCastRPC_RescueHooking(FName sectionName);

	// 뛸 때 왼발, 오른 발 사운드 재생 함수
	void PlayLeftSound();
	void PlayRightSound();

	// 다친 상태일 때 사운드 랜덤 반복 재생
	void PlayInjureSound();
	UFUNCTION()
	void OnInjureSoundFinished(); // 사운드 반복재생하는 델리게이트
	void StopInjureSound(); // 사운드 멈추는 함수

	// 맞을 때 소리 지르는 함수
	void PlayScreamSound();

	
	void PrintNetLog();

	float testCheckTime = 0;
	bool btest = false;
	float testRescueTime = 0;
};



