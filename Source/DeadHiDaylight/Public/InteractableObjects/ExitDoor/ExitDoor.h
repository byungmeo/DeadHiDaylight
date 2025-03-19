// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExitDoor.generated.h"

UCLASS()
class DEADHIDAYLIGHT_API AExitDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExitDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInteractionPoint> CamperPoint = nullptr;

	UFUNCTION()
	void OnInteraction(class UInteractionPoint* Point, AActor* OtherActor);
	UFUNCTION()
	void OnStopInteraction(class UInteractionPoint* Point, AActor* OtherActor);
};