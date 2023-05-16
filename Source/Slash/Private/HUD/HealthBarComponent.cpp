// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"


// Sets default values for this component's properties
UHealthBarComponent::UHealthBarComponent()
{
	
}


// Called when the game starts
void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();
	HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	
}

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}

