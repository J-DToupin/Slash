// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, instigatorActor, UAttributeComponent*, OwnimComp, float, Heath, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnStaminaChanged, AActor*, instigatorActor, UAttributeComponent*, OwnimComp, float, Stamina, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSoulChanged, AActor*, instigatorActor, UAttributeComponent*, OwnimComp, int32, Soul, int32, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGoldChanged, AActor*, instigatorActor, UAttributeComponent*, OwnimComp, int32, Gold, int32, Delta);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();

	UPROPERTY()
	FOnHealthChanged OnHealthChanged;
	UPROPERTY()
	FOnHealthChanged OnStaminaChanged;
	UPROPERTY()
	FOnSoulChanged OnSoulChanged;
	UPROPERTY()
	FOnSoulChanged OnGoldChanged;

private:

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Health{100};
	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxHealth{100};

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Stamina{100};
	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxStamina{100};

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	int32 Soul{0};
	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	int32 Gold{0};

public:
	void AddHealth(const float Delta);
	
	float GetPercentHealth() const;

	void AddStamina(const float Delta);
	
	float GetPercentStamina() const;

	float GetStamina() const;

	void AddSoul(const int32 Delta);
	
	void AddGold(const int32 Delta);

	bool IsAlive() const;

	bool IsTired() const;

	bool IsStaminaFull() const;
	
	[[nodiscard]] float GetMaxHealth() const
	{
		return MaxHealth;
	}

	void SetMaxHealth(float NewMaxHealth)
	{
		this->MaxHealth = NewMaxHealth;
	}

	int32 GetSoul() const
	{
		return Soul;
	}

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
};
