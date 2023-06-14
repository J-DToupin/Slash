// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

/**
 * 
 */

class UImage;
class UAttributeComponent;
class UProgressBar;
class UTextBlock;

UCLASS()
class SLASH_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealthBarPercent(float Percent);
	
	void SetStaminaBarPercent(float Percent);

	void SetGoldText(int32 Gold);

	void SetSoulText(int32 Soul);

	void HiddenCrossHair() const;
	void ShowCrossHair() const;

protected:
	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwnimComp, float Heath, float Delta);

	UFUNCTION()
	void OnStaminaChanged(AActor* InstigatorActor, UAttributeComponent* OwnimComp, float Stamina, float Delta);

	UFUNCTION()
	void OnSoulChanged(AActor* InstigatorActor, UAttributeComponent* OwnimComp, int32 Soul, int32 Delta);

	UFUNCTION()
	void OnGoldChanged(AActor* InstigatorActor, UAttributeComponent* OwnimComp, int32 Gold, int32 Delta);

	UFUNCTION()
	void OnAimActionPress(ASlashController* OwningComp, bool Press);
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HeathProgressBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> StaminaProgressBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> GoldText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SoulText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> CrossHair;
	
};
