// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CampfirePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ACampfirePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<class UUserWidget> CampfireWidget = nullptr;
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_AddWidget(TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(Client, Reliable)
	void ClientRPC_SetGuid(const FGuid NewGuid);

	void UpdateSlot(const int SlasherCount, const int CamperCount);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestSelect(const bool bIsSlasher);

	UFUNCTION(Client, Reliable)
	void ClientRPC_UpdateSelectedSlot(const bool bIsSlasher);
};
