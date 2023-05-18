// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class AAIController;
class UHealthBarComponent;
class UAttributeComponent;
class UAnimMontage;
UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	

private:

	UPROPERTY(EditAnywhere)
	double CombatRadius{500.f};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attribute;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;

	/**
	 * Navigation
	 */

	UPROPERTY()
	TObjectPtr<AAIController> AiController;

	// Current patrol target
	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius{200.f};

	void SetAI() const;
	
	/**
 *Animation Montage
 */

	FName DeathSectionName{};

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category=Sounds)
	TObjectPtr<USoundBase> HitSound;

	//la version cascade old one
	UPROPERTY(EditDefaultsOnly, Category=VisualEffects)
	TObjectPtr<UParticleSystem> HitParticle;

protected:
	virtual void BeginPlay() override;

	/**
	 * Play montage function
	*/
	void PLayMontage(const FName& SelectionName, UAnimMontage* Montage);
	
	void PlayRandomDeathMontage();

	void OnDeathMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	bool InTargetRange(const AActor* Target, const double Radius) const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	void Death();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
