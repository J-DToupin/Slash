// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/PickUpInterface.h"
#include "SlashCharacter.generated.h"



class UTimelineComponent;
class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;
class AItem;


UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	ASlashCharacter();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	//Stat

	UPROPERTY(EditAnywhere)
	int32 DodgeStamina{25};

	UPROPERTY(EditAnywhere)
	int32 StaminaSecond{5};

	UPROPERTY()
	FTimerHandle TimerStaminaRegen;
	
	/**
	 * @brief Components
	 */
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, Category=Hair)
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, Category=Hair)
	TObjectPtr<UGroomComponent> Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverLappingItem;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY()
	TArray<AActor*> TargetPossible;
	
	// Montage
	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> DodgeMontage;

	// TimeLine

	UPROPERTY()
	TObjectPtr<UTimelineComponent> TimelineComponent;
	
	FTimeline CurveFTimeline;
	
	UPROPERTY(EditAnywhere, Category=Timeline)
	TObjectPtr<UCurveFloat> CurveFloat;

	UPROPERTY(EditAnywhere, Category=Timeline)
	FVector StartCamera{};
	UPROPERTY(EditAnywhere, Category=Timeline)
	FVector EndCamera{150.f, 50.f,0};
	
protected:
	

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void Disarm();
	void Arm();

	void StaminaRegen() const;

public:

	//Timeline
	// void SetTimeLine();
	// UFUNCTION()
	// void TimeLineProgress(float Value);


	void Aim() const;
	void OffAim() const;


	
	virtual void Tick(float DeltaTime) override;

	virtual void SetOverlappingItem(AItem* Item) override;
	
	virtual void AddSouls(ASoul* Soul) override;

	virtual void AddGold(ATresor* Tresor) override;

	virtual void AddHealth(ATresor* Tresor) override;

	bool IsOccupied() const;

	bool PickUpWeapon();


	virtual void Jump() override;
	bool CanDoge();

	void Dodge();
	
	void SwitchWeapon();

	void SelectTargetPossible();

	virtual void SetTarget_Implementation(AActor* Actor) override;
};
