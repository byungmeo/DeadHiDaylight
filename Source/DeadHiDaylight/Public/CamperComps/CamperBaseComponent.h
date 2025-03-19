// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CamperBaseComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADHIDAYLIGHT_API UCamperBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCamperBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// 소유 엑터
	UPROPERTY()
	class ACamper* Camper;
	// 이동 컴포넌트
	UPROPERTY()
	class UCharacterMovementComponent* moveComp;

	virtual void SetupInputBinding(class UEnhancedInputComponent* pi) {};
};
