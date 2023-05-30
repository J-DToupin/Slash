// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "Characters/CharracterTypes.h"
#include "Component/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "Slash/DebugMacros.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Warping Component"));

	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute"));
	
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

	/**
	 * Play montage function
	*/

void ABaseCharacter::PLayAttackMontage()
{
	PLayRandomMontage(AttackSelection, AttackMontage);
}

void ABaseCharacter::PLayEquipMontage()
{
	PLayMontage(FName("Equip"), EquipMontage);
}

void ABaseCharacter::PLayUnEquipMontage()
{
	PLayMontage(FName("UnEquip"), EquipMontage);
}

void ABaseCharacter::PLayMontage(const FName& NameSelection, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (!AnimInstance && !Montage) return;
	
	bool bPlayedSuccessfully = PlayAnimMontage(Montage, 1.0f, NameSelection) > 0.0f;

	if (bPlayedSuccessfully)
	{
		DeathSectionName = NameSelection;

		// if (Montage == AttackMontage)
		// {
		// 	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABaseCharacter::OnMontageAnimNotify);
		// }
		
		if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveMontageInstance())
		{
			MontageInstance->OnMontageBlendingOutStarted.BindUObject(this, &ABaseCharacter::OnMontageBlendingOut);
		}
	}
	
}

void ABaseCharacter::StopMontage(UAnimMontage* Montage)
{

	StopAnimMontage(Montage);
	// UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// if (Montage && AnimInstance)
	// {
	// 	AnimInstance->Montage_Stop(0.25f, Montage);
	// 	
	// }
}

int32 ABaseCharacter::PLayRandomMontage(const TArray<FName>& ArraySelection, UAnimMontage* Montage)
{
	if (ArraySelection.IsEmpty()) return -1;

	const int32 SelectionIndex = FMath::RandRange(0,ArraySelection.Num() - 1);

	PLayMontage(ArraySelection[SelectionIndex], Montage);

	return SelectionIndex;
}

void ABaseCharacter::PlayDeathMontage()
{
	PLayRandomMontage(DeathSelection, DeathMontage);
}

void ABaseCharacter::OnDeathMontageBlendingOut()
{
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)

	{
		int32 SectionIndex = DeathMontage->GetSectionIndex(DeathSectionName);

		float SectionStart;

		float SectionEnd;

		DeathMontage->GetSectionStartAndEndTime(SectionIndex, SectionStart, SectionEnd);

		PlayAnimMontage(DeathMontage, 1.0f, DeathSectionName);
		
		AnimInstance->Montage_SetPosition(

		DeathMontage,

		SectionStart + DeathMontage->GetSectionLength(SectionIndex) - KINDA_SMALL_NUMBER);
		
		AnimInstance->Montage_SetPlayRate(DeathMontage, 0.0f);

		}
	
}

void ABaseCharacter::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted) return;

	if (Montage == DeathMontage)
	{
		OnDeathMontageBlendingOut();
	}
	else
	{
		MontageEnd();
	}
}

// void ABaseCharacter::OnMontageAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("AnimNotify"))
// 	if (NotifyName == FName("EnableBoxCollision"))
// 	{
// 		SetWeaponCollisionEnabled(ECollisionEnabled::QueryOnly);
// 	}
//
// 	if (NotifyName == FName("DisableBoxCollision"))
// 	{
// 		SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
// 	}
//
// 	//GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ABaseCharacter::OnMontageAnimNotify);
// }

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = cos(theta) si les deux Vector son Normal
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// Take the inverse cosine(Acos) of cosTheta to get theta ( angle entre les deux Vector 
	double Theta = FMath::Acos(CosTheta);

	// le metre en Degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("HitBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("HitFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135)
	{
		Section = FName("HitRight");
	}

	PLayMontage(Section, HitMontage);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
}

void ABaseCharacter::PLayHitSound(const FVector& ImpactPoint) const
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint) const
{
	if (HitParticle)
	{
		//version cascade
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),HitParticle, ImpactPoint);
	}
}

void ABaseCharacter::HandleDamage(const float DamageAmount)
{
	if (Attribute)
	{
		Attribute->AddHealth(-DamageAmount);
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget() const
{
	if (!CombatTarget) return FVector::Zero();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToThis = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToThis *= WarpTargetDistance;

	DRAW_SPHERE(CombatTargetLocation + TargetToThis, FColor::Purple)

	return  CombatTargetLocation + TargetToThis;
}

FVector ABaseCharacter::GetRotationWarpTarget() const
{
	if (!CombatTarget) return FVector::Zero();
	
	return CombatTarget->GetActorLocation();
}

bool ABaseCharacter::IsAlive() const
{
	return Attribute && Attribute->IsAlive();
}


/**
	 * State update
	 */


void ABaseCharacter::MontageEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}


bool ABaseCharacter::CanAttack() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterEquipState != ECharacterEquipState::ECS_Unequipped;
			
}

bool ABaseCharacter::CanDisarm() const
{
	return CharacterEquipState != ECharacterEquipState::ECS_Unequipped
	&& ActionState == EActionState::EAS_Unoccupied;
}

bool ABaseCharacter::CanArm() const
{
	return CharacterEquipState == ECharacterEquipState::ECS_Unequipped
	&& ActionState == EActionState::EAS_Unoccupied
	&& EquippedWeapon;
}

	/**
	 * Weapon Socket
	 */

void ABaseCharacter::PutWeaponBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("spine_Socket"));
	}
}

void ABaseCharacter::PutWeaponRightHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (WarpingComponent)
	{
		WarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("TranslationTarget"),GetTranslationWarpTarget());
		WarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("RotationTarget"),GetRotationWarpTarget());
	}
}


// Public function

bool ABaseCharacter::InTargetRange(const AActor* Target, const double Radius) const
{
	if (Target == nullptr) return false;
	
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint,  AActor* Hitter)
{
	
	//DRAW_SPHERE(ImpactPoint, FColor::Red)
	StopMontage(AttackMontage);
	if (IsAlive() && Hitter)
	{
		ActionState = EActionState::EAS_HitReact;
		DirectionalHitReact(Hitter->GetActorLocation());
		
	}

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	PLayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void ABaseCharacter::Dead()
{
	ActionState = EActionState::EAS_Dead;
	GetCharacterMovement()->Deactivate();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::Death()
{
	Dead();
	PlayDeathMontage();
}

void ABaseCharacter::Attack()
{
	if (CanAttack())
	{
		PLayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnable);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
