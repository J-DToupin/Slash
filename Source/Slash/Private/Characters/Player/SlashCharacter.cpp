// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SlashCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Component/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Soul.h"
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
	
	

	GetWorldTimerManager().SetTimer(TimerStaminaRegen,this, &ASlashCharacter::StaminaRegen, 1.0, true);
}

void ASlashCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASlashCharacter::OnBoxOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ASlashCharacter::OnBoxEndOverlap);
	}
	
}

void ASlashCharacter::Disarm()
{
	PlayUnEquipMontage();
	CharacterEquipState = ECharacterEquipState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage();
	CharacterEquipState = ECharacterEquipState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::StaminaRegen() const
{
	if (Attribute && Attribute->GetPercentStamina() < 1)
	{
		if (Attribute->IsStaminaFull())
		{
			GetWorldTimerManager().PauseTimer(TimerStaminaRegen);
		}
		else
		{
			Attribute->AddStamina(StaminaSecond);
		}
	}
}

// void ASlashCharacter::SetTimeLine()
// {
// 	if (CurveFloat)
// 	{
// 		FOnTimelineFloat TimelineFloat;
// 		TimelineFloat.BindUFunction(this, FName("TimeLineProgress"));
// 		CurveFTimeline.AddInterpFloat(CurveFloat, TimelineFloat);
//
// 		CurveFTimeline.PlayFromStart();
// 	}
// }
//
// void ASlashCharacter::TimeLineProgress(float Value)
// {
// 	
// 	if (SpringArmComponent)
// 	{
// 		const FVector NewLocation = FMath::Lerp(EndCamera, StartCamera, Value);
// 		SpringArmComponent->SocketOffset = NewLocation;
//
// 		const FVector End = EndCamera;
// 		EndCamera = StartCamera;
// 		StartCamera = End;
// 		
// 	}
// }

void ASlashCharacter::Aim() const
{
	if (SpringArmComponent)
	{
		SpringArmComponent->SocketOffset.Y = 70.f;
		SpringArmComponent->TargetArmLength = 150;
		
	}
}

void ASlashCharacter::OffAim() const
{
	if (SpringArmComponent)
	{
		SpringArmComponent->SocketOffset.Y = 0.f;
		SpringArmComponent->TargetArmLength = 300;
	}
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
		Attribute->AddGold(Tresor->GetValue());
	}
}

void ASlashCharacter::AddHealth(ATresor* Tresor)
{
	IPickUpInterface::AddHealth(Tresor);

	if (Attribute)
	{
		Attribute->AddHealth(Tresor->GetValue());
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
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		Super::Jump();
	}
}

bool ASlashCharacter::CanDoge()
{
	return DodgeMontage && Attribute && Attribute->GetStamina() > DodgeStamina && ActionState ==
		EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Dodge()
{
	if (CanDoge())
	{
		ActionState = EActionState::EAS_Dodging;
		PlayMontage(DodgeMontage);
		Attribute->AddStamina(-DodgeStamina);
		GetWorldTimerManager().UnPauseTimer(TimerStaminaRegen);
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

