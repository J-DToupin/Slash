// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, instigatorActor, UAttributeComponent*, OwnimComp, float, Heath, float, Delta);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

private:

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Health{100};

	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxHealth{100};

public:
	void AddHealth(const float Delta);

	UFUNCTION(BlueprintCallable)
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
