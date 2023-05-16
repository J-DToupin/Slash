// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AttributeComponent.h"


// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UAttributeComponent::AddHealth(const float Damage)
{
	Health = FMath::Clamp(Health + Damage, 0, MaxHealth);
}

float UAttributeComponent::GetPercentHealth() const
{
	return Health / MaxHealth;
}

bool UAttributeComponent::IsAlive() const
{
	return  Health > 0;
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

