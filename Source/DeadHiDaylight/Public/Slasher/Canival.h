// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Canival.generated.h"

UCLASS()
class DEADHIDAYLIGHT_API ACanival : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACanival();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const struct FInputActionValue& inputValue);
	void Look(const FInputActionValue& InputActionValue);
	UFUNCTION(CallInEditor)
	void LeftClick_Start();
	UFUNCTION(CallInEditor)
	void LeftClick_Complet();
	UFUNCTION(CallInEditor)
	void RightClick_Start();
	UFUNCTION(CallInEditor)
	void RightClick_Complet();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(
	class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* Camera = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Hammer = nullptr;

	UPROPERTY(EditDefaultsOnly )
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

	UFUNCTION()
	void OnHammerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void FindPoint();
	void Kick();

	
	//공격 타이머
	FTimerHandle RigthAttackTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category=Attack)
	float RigthAttackDelay = 3.0f;
	
	bool bIsAttacking; //공격 실행되었는지
	bool bIsCharging; //공격 실행되었는지
	
	//공격실행
	UFUNCTION()
	void RightAttack();
	

	UPROPERTY(EditAnywhere)
	class UCanivalAnim* Anim;

	UFUNCTION()
	void OnChainSawHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
					   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

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
	void AttachSurvivorToShourder(class ACamper* Survivor);
	


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCanivalUI> WidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UCanivalUI* ChainsawWidgetInstance;
	
};

