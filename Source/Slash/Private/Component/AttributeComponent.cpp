// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AttributeComponent.h"


// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UAttributeComponent::AddHealth(const float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
}

float UAttributeComponent::GetPercentHealth() const
{
	return Health / MaxHealth;
}

void UAttributeComponent::AddStamina(const float Delta)
{
	Stamina = FMath::Clamp(Stamina + Delta, 0, MaxStamina);

	OnStaminaChanged.Broadcast(nullptr, this, Stamina, Delta);
}

float UAttributeComponent::GetPercentStamina() const
{
	return Stamina / MaxStamina;
}

float UAttributeComponent::GetStamina() const
{
	return Stamina;
}

void UAttributeComponent::AddSoul(const int32 Delta)
{
	Soul += Delta;

	OnSoulChanged.Broadcast(nullptr, this, Soul, Delta);
}

void UAttributeComponent::AddGold(const int32 Delta)
{
	Gold += Delta;

	OnGoldChanged.Broadcast(nullptr, this, Gold, Delta);
}

bool UAttributeComponent::IsAlive() const
{
	return  Health > 0;
}

bool UAttributeComponent::IsTired() const
{
	return Stamina == 0;
}

bool UAttributeComponent::IsStaminaFull() const
{
	return Stamina == MaxStamina;
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}



void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

