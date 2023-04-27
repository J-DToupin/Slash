// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharracterTypes.h"
#include "SlashCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;
class AItem;


UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();

private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category=Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category=Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverLappingItem;

	/**
	 *Animation Montage
	 */

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	UAnimMontage* AttackMontage;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* PicUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* AttackAction;

	
	virtual void BeginPlay() override;

	/**
	 * Input function
	 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack();
	void PicUp();

	/**
	 * Play montage function
	*/
	void PLayAttackMontage() const;

public:	
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) {OverLappingItem = Item;}
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;}

};
