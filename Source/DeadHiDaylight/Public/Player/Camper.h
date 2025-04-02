#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camper.generated.h"

enum class ECamperHealth : uint8;
enum class ECamperMoveState : uint8;
enum class ECamperStanceState : uint8;
enum class ECamperInteraction : uint8;
UCLASS()
class DEADHIDAYLIGHT_API ACamper : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACamper();

protected:
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
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

	// Injure sound 반복재생 세팅하는 타이머
	FTimerHandle injureTimerHandle;
	
	// HookIn Sound
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundCue* hookInCue;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimSound")
	class USoundAttenuation* hookInAttenuation;
	
	// 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* hitcrawlMontage;
	
	// moveComp
	UPROPERTY()
	class UCharacterMovementComponent* moveComp; // 캐릭터 움직임 컴포넌트
	UPROPERTY()
	class UCamperAnimInstance* Anim = nullptr;
	UPROPERTY()
	class ASacrificePlayerState* userState = nullptr;

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

	// 죽었는지 살았는지
	bool bIsDead = false;
	
	// 포인트 찾았는지 체크하는 변수
	bool bFindPoints = false;

	// 이동 관련 함수
	void CamperMove(const struct FInputActionValue& value); // 캠퍼 움직임 함수
	void StopCamperMove(const struct FInputActionValue& value);

	void StartRun(const struct FInputActionValue& value); // 캠퍼 뛰기 시작 함수
	void StopRun(const struct FInputActionValue& value);
	
	void Start_Crouch(const struct FInputActionValue& value); // 앉기 시작 함수 
	void End_Crouch(const struct FInputActionValue& value); // 앉기 끝 함수

	// 자세 상태 관리 함수
	void SetStanceState(ECamperStanceState NewState);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetStanceState(ECamperStanceState NewState);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_SetStanceState(ECamperStanceState NewState);
	// 움직임 상태 관리 함수
	void SetMovementState(ECamperMoveState NewState);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetMovementState(ECamperMoveState NewState);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_SetMovementState(ECamperMoveState NewState);
	// 상호작용 상태 관리 함수
	void SetInteractionState(ECamperInteraction NewState);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetInteractionState(ECamperInteraction NewState);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_SetInteractionState(ECamperInteraction NewState);
	// 건강 상태 관리 함수
	void SetHealthState(ECamperHealth NewState);
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetHealthState(ECamperHealth NewState);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_SetHealthState(ECamperHealth NewState);
	// 카메라 관련 함수
	void Look(const struct FInputActionValue& value);  // 카메라 움직임 함수

public:
	// 발전기 시작 / 실패
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StartRepair();
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_EndRepair();

	void FailRepair(FName sectionName);

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
	void ChangeSpeed();
	// 쓰러진 상태 함수 RPC
	void Crawling();

	// 문 여는 함수 RPC
	void StartUnLock();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StartUnLock();
	// 문 닫는 함수 RPC
	void EndUnLock();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_EndUnLock();
	// 갈고리 걸리는 함수 RPC
	void Hooking(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_Hooking(FName sectionName);
	// 갈고리에서 구해주는 RPC
	void RescueHooking(FName sectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_RescueHooking(FName sectionName);
	void CheckRescueTime(ACamper* camper);
	// 살인마가 드는 RPC
	void PickUp();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PickUp();
	// 살인마가 떨어트리는 RPC
	void PickUpDrop();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PickUpDrop();
	// 판자 내리는 RPC
	void PullDownPallet();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_PullDownPallet();
	// 쓰러지거나 상대 치유할 때 사용할때 힐 시작하는 RPC
	void StartHealing();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartHealing();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StartHealing();
	// 힐 그만하는 RPC
	void StopHealing();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopHealing();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StopHealing();

	// 힐시간 체크 함수
	void HealingTimingCheck(float deltaTime);
	UFUNCTION(Server, Reliable)
	void ServerRPC_HealingTimingCheck(float deltaTime);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_HealingTimingCheck(float deltaTime);

	// 힐링 시작 애니메이션 동기화 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartHealingAnimation(ACamper* camper);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StartHealingAnimation(ACamper* camper);
	
	// 힐링 끝 애니메이션 동기화 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_EndHealingAnimation(ACamper* camper);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_EndHealingAnimation(ACamper* camper);
	
	float curhealingTime = 0;
	float healingTime = 5;
	
	// 뛸 때 왼발, 오른 발 사운드 재생 함수
	void PlayLeftSound();
	void PlayRightSound();

	// 다친 상태일 때 사운드 랜덤 반복 재생
	void PlayInjureSound();

	// 맞을 때 소리 지르는 함수
	void PlayScreamSound();

	// 후크 걸릴 때 소리 지르는 함수
	void PlayHookInSound();

	
	
	// 캐릭터 Log 띄워주는 함수
	void PrintNetLog();

	float testCheckTime = 0;
	bool btest = false;
	float testRescueTime = 0;
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_HealthCheck();
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_HealthCheck();

	/*
	 *	살인마가 업을 수 있게 InteractionPoint 작업
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInteractionPoint> CrawlPoint = nullptr;
	UFUNCTION()
	void OnInteraction(class UInteractionPoint* Point, AActor* OtherActor);
	UFUNCTION()
	void OnStopInteraction(class UInteractionPoint* Point, AActor* OtherActor);


	/*
	 *	상호작용 관련
	 */
	// 현재 상호작용 시작이 가능한 Point
	UPROPERTY()
	class UInteractionPoint* NearPoint = nullptr;
	void ServerOnly_FindInteractionPoint();
	UFUNCTION(Client, Reliable)
	void ClientRPC_ChangeNearPoint(class UInteractionPoint* NewPoint);
	
	void TryInteraction();
	UFUNCTION(Server, Reliable)
	void ServerRPC_TryInteraction();
	
	void CheckInteractPoint();
	UFUNCTION(Server, Reliable)
	void ServerRPC_CheckInteractPoint();
	
	void StopInteract();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopInteract();
	
	// 현재 상호작용을 진행 중인 Point
	UPROPERTY()
	class UInteractionPoint* InteractingPoint = nullptr;
	
	void OnRescued();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_OnRescued();
};