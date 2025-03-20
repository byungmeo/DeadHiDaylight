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
	void Kick(const struct FInputActionValue& inputValue);
	void PlayKick(const FInputActionValue& inputValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(
	class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* imc_carnival;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_move;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_kick;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UAnimMontage* MyAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	float speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	float direction = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	bool CanAttack  = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	bool Input_LMB  = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	bool ElapsedTime   = false;
};
