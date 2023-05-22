// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"

#include "Characters/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (BaseCharacter)
	{
		CharacterMovementComponent = BaseCharacter->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);

		bIsFalling = CharacterMovementComponent->IsFalling();

		CharacterState = BaseCharacter->GetCharacterState();
	}
}
