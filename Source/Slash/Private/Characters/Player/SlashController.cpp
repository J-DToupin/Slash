// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SlashController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Player/SlashCharacter.h"

ASlashController::ASlashController()
{
	
}

void ASlashController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext,0);
	}

	SlashCharacter = Cast<ASlashCharacter>(GetCharacter());
}

void ASlashController::Move(const FInputActionValue& Value)
{
	if (SlashCharacter->IsOccupied())
	{
		return;
	}
	
	if (Value.GetMagnitude() != 0.0f)
	{
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

		//find out which way is forward
		const FVector DirectionForward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//find out which way is right
		const FVector DirectionRight = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		
		SlashCharacter->AddMovementInput(DirectionRight, Value[0]);
		SlashCharacter->AddMovementInput(DirectionForward, Value[1]);
		
	}
}

void ASlashController::Look(const FInputActionValue& Value)
{
	
	AddYawInput(Value[0]);
	AddPitchInput(Value[1]);
}

void ASlashController::EKeyPressed()
{
	if (!SlashCharacter->PickUpWeapon())
	{
		SlashCharacter->SwitchWeapon();
	}

}

void ASlashController::SelectTarget()
{
	SlashCharacter->SelectTargetPossible();
}

void ASlashController::Jump()
{
	SlashCharacter->Jump();
}

void ASlashController::StopJumping()
{
	SlashCharacter->StopJumping();
}

void ASlashController::Attack()
{
	SlashCharacter->Attack();
}

void ASlashController::Dodge()
{
	SlashCharacter->Dodge();
}

void ASlashController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashController::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashController::Look);
		
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ASlashController::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASlashController::StopJumping);

		Input->BindAction(PicUpAction, ETriggerEvent::Started, this, &ASlashController::EKeyPressed);
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &ASlashController::Attack);
		Input->BindAction(DodgeAction, ETriggerEvent::Started, this, &ASlashController::Dodge);

		Input->BindAction(TargetAction, ETriggerEvent::Started, this, &ASlashController::SelectTarget);
	}
}
