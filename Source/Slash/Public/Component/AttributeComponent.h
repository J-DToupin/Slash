﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();

private:

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Health{100};

	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxHealth{100};

public:
	void AddHealth(const float Damage);

	float GetPercentHealth() const;

	bool IsAlive() const;
	
	[[nodiscard]] float GetMaxHealth() const
	{
		return MaxHealth;
	}

	void SetMaxHealth(float NewMaxHealth)
	{
		this->MaxHealth = NewMaxHealth;
	}

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
};
