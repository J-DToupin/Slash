// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

class UPlayerOverlay;

UCLASS()
class SLASH_API ASlashHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASlashHUD();

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category=Overlay)
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	TObjectPtr<UPlayerOverlay> PlayerOverlay;

public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() const {return PlayerOverlay;}
	
};
