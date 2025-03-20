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
	void LeftClick();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(
	class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* Camera = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Hammer = nullptr;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* imc_carnival;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_move;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputAction> ia_look;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	const UInputAction* ia_leftClick;

	UPROPERTY()
	class UCanivalAnim* AnimInstance = nullptr;

public:
	UFUNCTION()
	void OnHammerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
