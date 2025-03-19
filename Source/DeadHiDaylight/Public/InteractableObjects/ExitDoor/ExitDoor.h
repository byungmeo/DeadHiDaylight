// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExitDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenExitDoor);

UCLASS()
class DEADHIDAYLIGHT_API AExitDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExitDoor();

private:
	// 전원 공급까지 20초 소요
	float BasePowerValue = 0.05f;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInteractionPoint> CamperPoint = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Test")
	bool bDoorOpened = false;
	UPROPERTY(BlueprintAssignable)
	FOnOpenExitDoor OnOpenExitDoor;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Test")
	bool bIsDoorOpening = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Test")
	float PowerGauge = 0.0f;
	void OpenExitDoor();
	
	UFUNCTION()
	void OnInteraction(class UInteractionPoint* Point, AActor* OtherActor);
	UFUNCTION()
	void OnStopInteraction(class UInteractionPoint* Point, AActor* OtherActor);
};