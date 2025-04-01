// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pallet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFallGround);

UCLASS()
class DEADHIDAYLIGHT_API APallet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APallet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USkeletalMeshComponent> Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BrokenMesh = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInteractionPoint> AttachPoint1 = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInteractionPoint> AttachPoint2 = nullptr;

	UFUNCTION()
	void OnInteraction(class UInteractionPoint* Point, AActor* OtherActor);

	bool bIsFallOnGround = false;
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Test")
	void FallGround();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_FallGround();
	FOnFallGround OnFallGround;

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Test")
	void Broken(class UInteractionPoint* Point);
};
