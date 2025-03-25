// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SacrificePlayerController.h"
#include "GameFramework/PlayerController.h"
#include "CampfirePlayerController.generated.h"

enum class EPlayerRole : uint8;
/**
 * 
 */
UCLASS()
class DEADHIDAYLIGHT_API ACampfirePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	FGuid Guid;
	
	UPROPERTY(ReplicatedUsing = OnRep_SelectedRole)
	EPlayerRole SelectedRole = EPlayerRole::EPR_None;
	UFUNCTION()
	void OnRep_SelectedRole() const;
	
	UPROPERTY()
	TObjectPtr<class UUserWidget> CampfireWidget = nullptr;
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_AddWidget(TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(Client, Reliable)
	void ClientRPC_SetGuid(const FGuid NewGuid);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestSelect(const EPlayerRole ReqRole);

	void UpdateSlot(const int SlasherCount, const int CamperCount);
	
	void UpdateSelectedSlot(const bool bIsSlasher);
};
