// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SlashCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Component/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Soul.h"
#include "Items/Tresor/Heart.h"
#include "Items/Tresor/Tresor.h"
#include "Items/Weapons/Weapon.h"


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
	if (CombatTarget == nullptr && EventInstigator->GetPawn()->Implements<UTargetInterface>())
	{
		CombatTarget = EventInstigator->GetPawn();

		ITargetInterface::Execute_SetVisibilityTargetArrow(EventInstigator->GetPawn(), true);
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
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASlashCharacter::OnBoxOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ASlashCharacter::OnBoxEndOverlap);
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

void ASlashCharacter::SetOverlappingItem(AItem* Item)
{
	IPickUpInterface::SetOverlappingItem(Item);

	OverLappingItem = Item;
}

void ASlashCharacter::AddSouls(ASoul* Soul)
{
	IPickUpInterface::AddSouls(Soul);

	if (Attribute)
	{
		Attribute->AddSoul(Soul->GetSoulCount());
	}
	
}

void ASlashCharacter::AddGold(ATresor* Tresor)
{
	IPickUpInterface::AddGold(Tresor);

	if (Attribute)
	{
		Attribute->AddGold(Tresor->GetGold());
	}
}

void ASlashCharacter::AddHeart(AHeart* Heart)
{
	IPickUpInterface::AddHeart(Heart);

	if (Attribute)
	{
		Attribute->AddHealth(Heart->GetHealthCount());
	}
}

bool ASlashCharacter::IsOccupied() const
{
	return ActionState != EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::PickUpWeapon()
{
	if (AWeapon* OverLapinWeapon = Cast<AWeapon>(OverLappingItem))
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->UnEquip(OverLapinWeapon);
		}
		
		OverLapinWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this, true);
		CharacterEquipState = ECharacterEquipState::ECS_EquippedOneHandedWeapon;
		OverLappingItem = nullptr;
		EquippedWeapon = OverLapinWeapon;

		return true;
	}

	return false;
}

void ASlashCharacter::Jump()
{
	if (ActionState != EActionState::EAS_Dead)
	{
		Super::Jump();
	}
}

void ASlashCharacter::SwitchWeapon()
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

void ASlashCharacter::SelectTargetPossible()
{
	
	if (TargetPossible.IsEmpty()) return;

	if (CombatTarget && CombatTarget->Implements<UTargetInterface>())
	{
		ITargetInterface::Execute_SetVisibilityTargetArrow(CombatTarget, false);
	}

	if (TargetPossible.Num() == 1)
	{
		CombatTarget = TargetPossible[0];
	}
	else
	{
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
	}

	if (CombatTarget && CombatTarget->Implements<UTargetInterface>())
	{
		ITargetInterface::Execute_SetVisibilityTargetArrow(CombatTarget, true);
	}
	//UKismetSystemLibrary::LineTraceSingle()
}

void ASlashCharacter::SetTarget_Implementation(AActor* Actor)
{
	Super::SetTarget_Implementation(Actor);

	SetCombatTarget(Actor);
	
}

