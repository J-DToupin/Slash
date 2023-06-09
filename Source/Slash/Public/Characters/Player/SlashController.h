﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SlashController.generated.h"

class UPlayerOverlay;
class ASlashCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAimActionPress, AActor*, InstigatorActor, ASlashController*, OwningComp, bool, Press);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAimActionPressSignature, class ASlashController*, bool);

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

	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputAction> AimAction;


	UPROPERTY()
	TObjectPtr<UPlayerOverlay> PlayerOverlay;

	bool IsAiming{};

public:
	ASlashController();

	
	FOnAimActionPressSignature OnAimActionPressDelegate;

protected:
	virtual void BeginPlay() override;
	void DefaultMove(const FInputActionValue& Value);


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

	
	void Aim();
	void OffAim();

	virtual void SetupInputComponent() override;
};
