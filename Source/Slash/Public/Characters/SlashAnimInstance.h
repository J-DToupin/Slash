// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharracterTypes.h"
#include "SlashAnimInstance.generated.h"

class AEnemy;
class ABaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ABaseCharacter> BaseCharacter;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEnemy> EnemyCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category="Movement | Character State")
	ECharacterEquipState CharacterEquipState;
	
};
