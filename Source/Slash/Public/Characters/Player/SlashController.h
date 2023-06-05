// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SlashController.generated.h"

class ASlashCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SLASH_API ASlashController : public APlayerController
{
	GENERATED_BODY()

	TObjectPtr<ASlashCharacter> SlashCharacter;

	UPROPERTY(EditAnywhere,  Category= "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere,  Category= "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere,  Category= "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere,  Category= "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere,  Category= "Input")
	TObjectPtr<UInputAction> PicUpAction;

	UPROPERTY(EditAnywhere,  Category= "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputAction> TargetAction;

	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputAction> DodgeAction;


public:
	ASlashController();

protected:
	virtual void BeginPlay() override;
	

	/**
 * Input function
 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);;
	void EKeyPressed();
	void SelectTarget();
	void Jump();
	void StopJumping();
	void Attack();
	void Dodge();

	virtual void SetupInputComponent() override;
	
};
