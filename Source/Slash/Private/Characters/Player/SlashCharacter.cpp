// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SlashCharacter.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
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

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Vision Target"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	if (CombatTarget == nullptr)
	{
		CombatTarget = EventInstigator->GetPawn();
	}

	if (!IsAlive())
	{
		Death();
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASlashCharacter::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		TargetPossible.AddUnique(OtherActor);
		GEngine->AddOnScreenDebugMessage(6, 0.01f, FColor::Green, TEXT("InBox"));
	}
}

void ASlashCharacter::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		TargetPossible.Remove(OtherActor);
		GEngine->AddOnScreenDebugMessage(6, 0.01f, FColor::Red, TEXT("OutBox"));

	}
}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("PlayerCharacter"));

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext,0);
		}
	}

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASlashCharacter::OnBoxOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ASlashCharacter::OnBoxEndOverlap);
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

void ASlashCharacter::Disarm()
{
	PLayUnEquipMontage();
	CharacterEquipState = ECharacterEquipState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Arm()
{
	PLayEquipMontage();
	CharacterEquipState = ECharacterEquipState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::EKeyPressed()
{
	
	if (AWeapon* OverLappinWeapon = Cast<AWeapon>(OverLappingItem))
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->UnEquip(OverLappinWeapon);
		}
		
		OverLappinWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this, true);
		CharacterEquipState = ECharacterEquipState::ECS_EquippedOneHandedWeapon;
		OverLappingItem = nullptr;
		EquippedWeapon = OverLappinWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void ASlashCharacter::SelectTarget()
{
	if (TargetPossible.IsEmpty()) return;

	if (TargetPossible.Num() == 1)
	{
		CombatTarget = TargetPossible[0];
		return;
	}

	int32 MinDistance{999999};

	for (AActor* Actor : TargetPossible)
	{
		const int32 Distance = FVector::Distance(Actor->GetActorLocation(), GetActorLocation());
		
		if (MinDistance > Distance)
		{
			MinDistance = Distance;
			CombatTarget = Actor;
		}
		
	}
	
	//UKismetSystemLibrary::LineTraceSingle()
}


// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		GEngine->AddOnScreenDebugMessage(5, 0.01f, FColor::Blue, CombatTarget->GetName());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(5, 0.01f, FColor::Blue, TEXT("PtrNull"));
	}

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
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &ABaseCharacter::Attack);
		
		Input->BindAction(TargetAction, ETriggerEvent::Started, this, &ASlashCharacter::SelectTarget);
	}

}

