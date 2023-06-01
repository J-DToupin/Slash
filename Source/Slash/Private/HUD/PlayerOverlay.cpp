// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"

#include "Component/AttributeComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerOverlay::SetHealthBarPercent(float Percent)
{
	if (HeathProgressBar)
	{
		HeathProgressBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetGoldText(int32 Gold)
{
	if (GoldText)
	{
		GoldText->SetText(FText::AsNumber(Gold));
	}
}

void UPlayerOverlay::SetSoulText(int32 Soul)
{
	if (SoulText)
	{
		SoulText->SetText(FText::AsNumber(Soul));
	}
}

void UPlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	UAttributeComponent* AttributeComponent =  Cast<UAttributeComponent>(GetOwningPlayerPawn()->FindComponentByClass(UAttributeComponent::StaticClass()));

	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &UPlayerOverlay::OnHealthChanged);
	}
}

void UPlayerOverlay::OnHealthChanged(AActor* instigatorActor, UAttributeComponent* OwnimComp, float Heath, float Delta)
{
	SetHealthBarPercent(OwnimComp->GetPercentHealth());
}
