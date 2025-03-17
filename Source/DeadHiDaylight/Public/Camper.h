#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Camper.generated.h"

UCLASS()
class DEADHIDAYLIGHT_API ACamper : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACamper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
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
	


	
	UPROPERTY()
	class UCharacterMovementComponent* moveComp; // 캐릭터 움직임 컴포넌트
	UPROPERTY(EditAnywhere)
	class UCamperAnimInstance* Anim;
	
	// Input 변수
	UPROPERTY(EditAnywhere)
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
	
	
	// 속도 변수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float moveSpeed = 226; // 걷는 속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float maxSpeed = 400; // 뛰는 속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camper)
	float crouchSpeed = 113; // 앉아서 걷는 속도

	// 이동 관련 함수
	void CamperMove(const FInputActionValue& value); // 캠퍼 움직임 함수
	void Run(const struct FInputActionValue& value); // 캠퍼 뛰는 함수
	void Start_Crouch(const struct FInputActionValue& value); // 앉기 시작 함수 
	void End_Crouch(const struct FInputActionValue& value); // 앉기 끝 함수
	// 카메라 관련 함수
	void Look(const struct FInputActionValue& value);  // 카메라 움직임 함수

	/*
	 * Temp for Interact with Generator
	 */
public:
	// 여러 함수를 처리할 함수 바인딩이 필요 할듯
	void StartRepair();
	void EndRepair();
};

