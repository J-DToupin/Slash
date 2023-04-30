// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Animation/AnimInstance.h"
#include "Items/Weapons/Weapon.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArmComponent);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext,0);
		}
	}
	
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied)
	{
		return;
	}
	
	if (Value.GetMagnitude() != 0.0f && GetController())
	{
		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

		//find out which way is forward
		const FVector DirectionForward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//find out which way is right
		const FVector DirectionRight = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(DirectionRight, Value[0]);
		AddMovementInput(DirectionForward, Value[1]);
	}
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	if (GetController())
	{
		AddControllerYawInput(Value[0]);
		AddControllerPitchInput(Value[1]);
	}
}

void ASlashCharacter::Attack()
{
	if (CanAttack())
	{
		PLayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	
}

void ASlashCharacter::EKeyPressed()
{
	
	if (AWeapon* OverLappinWeapon = Cast<AWeapon>(OverLappingItem))
	{
		OverLappinWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverLappingItem = nullptr;
		EquippedWeapon = OverLappinWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PLayEquipMontage(FName("UnEquip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanArm())
		{
			PLayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void ASlashCharacter::PLayAttackMontage() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0,1);
		FName SelectionName;
		switch (Selection)
		{
		case 0:
			SelectionName = "Attack1";
			break;

		case 1:
			SelectionName = "Attack2";
			break;

		default:
			SelectionName = "Attack1";
			break;
		}

		AnimInstance->Montage_JumpToSection(SelectionName, AttackMontage);
	}
}

void ASlashCharacter::PLayEquipMontage(FName SelectionName) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SelectionName, EquipMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanAttack() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanDisarm() const
{
	return CharacterState != ECharacterState::ECS_Unequipped
	&& ActionState == EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanArm() const
{
	return CharacterState == ECharacterState::ECS_Unequipped
	&& ActionState == EActionState::EAS_Unoccupied
	&& EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("spine_Socket"));
	}
}

void ASlashCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		Input->BindAction(PicUpAction, ETriggerEvent::Started, this, &ASlashCharacter::EKeyPressed);
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &ASlashCharacter::Attack);
	}

}

