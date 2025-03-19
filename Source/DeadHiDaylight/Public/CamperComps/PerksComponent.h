// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CamperComps/CamperBaseComponent.h"
#include "PerksComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADHIDAYLIGHT_API UPerksComponent : public UCamperBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPerksComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* pi) override;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_DeadHard;


	float beforeSpeed = 0;
	void PerksDeadHard();
	void DeadHardTimer();
};
