﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharracterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"


class UAttributeComponent;
class AWeapon;
class UAnimMontage;
class UMotionWarpingComponent;

UCLASS(Abstract)
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

private:

	
	/**
	*Animation Montage
	*/

	FName DeathSectionName{};

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TArray<FName> AttackSelection;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TArray<FName> DeathSelection;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess), Category=Montages)
	TObjectPtr<UMotionWarpingComponent> WarpingComponent;

	UPROPERTY(EditAnywhere, Category=Sounds)
	TObjectPtr<USoundBase> HitSound;

	//la version cascade old one
	UPROPERTY(EditDefaultsOnly, Category=VisualEffects)
	TObjectPtr<UParticleSystem> HitParticle;

	

	
	

protected:

	// Enum State

	
	ECharacterEquipState CharacterEquipState = ECharacterEquipState::ECS_Unequipped;
	
	UPROPERTY(BlueprintReadWrite)
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/**
 * @brief Components
 */
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attribute;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> EquippedWeapon;
	
	UPROPERTY(BlueprintReadOnly, Category=Combat)
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, Category=Combat)
	double WarpTargetDistance{75.f};

	
	virtual void BeginPlay() override;

	/**
	 * Play montage function
	*/
	
	virtual void PLayAttackMontage();
	
	void PLayEquipMontage();
	
	void PLayUnEquipMontage();
	
	void PLayMontage(const FName& NameSelection, UAnimMontage* Montage);

	void StopMontage(UAnimMontage* Montage);
	
	int32 PLayRandomMontage(const TArray<FName>& ArraySelection, UAnimMontage* Montage);
	
	void PlayDeathMontage();

	void OnDeathMontageBlendingOut();

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	// UFUNCTION()
	// void OnMontageAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	
	
	void DirectionalHitReact(const FVector& ImpactPoint);

	void PLayHitSound(const FVector& ImpactPoint) const;

	void SpawnHitParticles(const FVector& ImpactPoint) const;

	virtual void HandleDamage(const float DamageAmount);

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget() const;

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget() const;

	/**
	 * State update
	 */

	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable)
	virtual void MontageEnd();
	
	virtual bool CanAttack() const;

	bool CanDisarm() const;
	bool CanArm() const;
	
	/**
	 * Weapon Socket
	 */

	UFUNCTION(BlueprintCallable)
	void PutWeaponBack();

	UFUNCTION(BlueprintCallable)
	void PutWeaponRightHand();


public:

	virtual void Tick(float DeltaTime) override;

	bool InTargetRange(const AActor* Target, const double Radius) const;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	void Dead();
	virtual void Death();
	virtual void Attack();
	
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable);

	FORCEINLINE ECharacterEquipState GetCharacterEquipState() const {return CharacterEquipState;}
};
