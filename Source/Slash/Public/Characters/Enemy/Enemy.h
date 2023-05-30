// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharracterTypes.h"
#include "Enemy.generated.h"


class AWeapon;
class UPawnSensingComponent;
class AAIController;
class UHealthBarComponent;


UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;
	

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void PawnSeen(APawn* Target);

	AActor* ChoosePatrolTarget();
	
	virtual bool CanAttack() const override;
	virtual void Attack() override;
	virtual void MontageEnd() override;
	
	virtual void HandleDamage(const float DamageAmount) override;
	virtual void Death() override;
	
private:
	
	void InitializeEnemy();
	// AI Behavior
	void CheckCombatTarget();
	void CheckPatrolTarget();

	void SetAIMoveToTarget(const AActor* Target) const;
	void PatrolTimerFinished() const;
	
	void HideHealthBar() const;
	void ShowHealthBar() const;
	
	void LoseInterest();
	void StartPatrolling();
	void StartChasing();
	void StartAttacking();

	bool IsInsideCombatRadius() const;
	bool IsInsideAttackRadius() const;

	bool IsDeath() const;
	bool IsPatrolling() const;
	bool IsChasing() const;
	bool IsEngaged() const;
	bool IsAttacking() const;

	void ClearPatrolTimer();
	void ClearAttackTimer();
	
	// Stats

	UPROPERTY(EditAnywhere)
	float PatrollingSpeed = 125.f;
	
	UPROPERTY(EditAnywhere)
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere)
	float LifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category=Combat)
	double CombatRadius{1000.f};

	UPROPERTY(EditAnywhere, Category=Combat)
	double AttackRadius{150.f};

	//Combat
	
	FTimerHandle AttackTimer{};

	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackMax = 1.f;
	
	/**
	 * @brief Components
	 */
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;
	
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

	FTimerHandle PatrolTimer;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	float MinWaitPatrol = 5.f;
	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	float MaxWaitPatrol = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
};
