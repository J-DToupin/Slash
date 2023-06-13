// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"

#include "Characters/Player/SlashController.h"
#include "Component/AttributeComponent.h"
#include "Components/Image.h"
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

void UPlayerOverlay::HiddenCrossHair() const
{
	if (CrossHair)
	{
		CrossHair->SetVisibility(ESlateVisibility::Hidden);

	}
}

void UPlayerOverlay::ShowCrossHair() const
{
	if (CrossHair)
	{
		CrossHair->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	UAttributeComponent* AttributeComponent =  Cast<UAttributeComponent>(GetOwningPlayerPawn()->FindComponentByClass(UAttributeComponent::StaticClass()));

	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &UPlayerOverlay::OnHealthChanged);
		AttributeComponent->OnStaminaChanged.AddDynamic(this, &UPlayerOverlay::OnStaminaChanged);
		AttributeComponent->OnSoulChanged.AddDynamic(this, &UPlayerOverlay::OnSoulChanged);
		AttributeComponent->OnGoldChanged.AddDynamic(this, &UPlayerOverlay::OnGoldChanged);
	}

	ASlashController* SlashController = Cast<ASlashController>(GetOwningPlayerPawn()->Controller);

	if (SlashController)
	{
		SlashController->OnAimActionPress.AddDynamic(this, &UPlayerOverlay::OnAimActionPress);
	}

	HiddenCrossHair();
}

void UPlayerOverlay::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwnimComp, float Heath, float Delta)
{
	SetHealthBarPercent(OwnimComp->GetPercentHealth());
}

void UPlayerOverlay::OnStaminaChanged(AActor* InstigatorActor, UAttributeComponent* OwnimComp, float Stamina,
	float Delta)
{
	SetStaminaBarPercent(OwnimComp->GetPercentStamina());
}

void UPlayerOverlay::OnSoulChanged(AActor* InstigatorActor, UAttributeComponent* OwnimComp, int32 Soul, int32 Delta)
{
	SetSoulText(Soul);
}

void UPlayerOverlay::OnGoldChanged(AActor* InstigatorActor, UAttributeComponent* OwnimComp, int32 Gold, int32 Delta)
{
	SetGoldText(Gold);
}

void UPlayerOverlay::OnAimActionPress(AActor* InstigatorActor, ASlashController* OwinComp, bool Press)
{
	
	if (Press)
	{
		ShowCrossHair();
	}
	else
	{
		HiddenCrossHair();

	}
}
