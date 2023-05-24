﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Enemy.h"

#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Component/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SightRadius = 4000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	HealthBarWidget->SetVisibility(false);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}


void AEnemy::HideHealthBar() const
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar() const
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	SetAIMoveToTarget(PatrolTarget);
}

void AEnemy::StartChasing()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	SetAIMoveToTarget(CombatTarget);
}

void AEnemy::StartAttacking()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

bool AEnemy::IsInsideCombatRadius() const
{
	return InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsInsideAttackRadius() const
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsDeath() const
{
	return EnemyState == EEnemyState::EES_Death;
}

bool AEnemy::IsPatrolling() const
{
	return EnemyState == EEnemyState::EES_Patrolling;
}

bool AEnemy::IsChasing() const
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsEngaged() const
{
	return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemy::IsAttacking() const
{
	return EnemyState == EEnemyState::EES_Attacking;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}


void AEnemy::HandleDamage(const float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attribute->GetPercentHealth());
		HealthBarWidget->SetVisibility(true);
	}
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AiController = Cast<AAIController>(GetController());

	SetAIMoveToTarget(PatrolTarget);

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
		
	}

	AWeapon* DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	EquippedWeapon = DefaultWeapon;
}


void AEnemy::SetAIMoveToTarget(const AActor* Target) const
{
	// Set AI Enemy
	if (AiController == nullptr || Target == nullptr) return;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(60.f);

	// FNavPathSharedPtr pointer qui store les data de Moveto
	AiController->MoveTo(MoveRequest);
	
}

void AEnemy::PatrolTimerFinished()
{
	SetAIMoveToTarget(PatrolTarget);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	if (PatrolTargets.IsEmpty()) return nullptr;

	int32 TargetSelection;
	
	do
	{
		 TargetSelection = FMath::RandRange(0, PatrolTargets.Num() - 1);
	}
	while (PatrolTarget == PatrolTargets[TargetSelection]);
	
	return PatrolTargets[TargetSelection];
}


void AEnemy::CheckCombatTarget()
{
	if (!IsInsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		
		if (!IsEngaged())
		{
			StartPatrolling();
		}
		
	}
	else if (!IsInsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();

		if (!IsEngaged())
		{
			StartChasing();
		}
		
	}
	else if (CanAttack())
	{
		StartAttacking();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		GEngine->AddOnScreenDebugMessage(3,1.f, FColor::Red, PatrolTarget->GetName());
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished,
			FMath::RandRange(MinWaitPatrol, MaxWaitPatrol));
	}
}

void AEnemy::PawnSeen(APawn* Target)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Death &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
			Target->ActorHasTag(FName("SlashCharacter"));

	if (bShouldChaseTarget)
	{
		CombatTarget = Target;
		ClearPatrolTimer();
		StartChasing();
	}
}

bool AEnemy::CanAttack() const
{
	return IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDeath();
}

void AEnemy::Attack()
{
	EnemyState = EEnemyState::EES_Engaged;
	PLayAttackMontage();
}

void AEnemy::AttackEnd()
{
	Super::AttackEnd();
	EnemyState = EEnemyState::EES_None;
	CheckCombatTarget();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsDeath()) return;
	
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);
	
}

void AEnemy::Death()
{
	Super::Death();
	EnemyState = EEnemyState::EES_Death;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	ClearAttackTimer();
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	HealthBarWidget->DestroyComponent();
	SetLifeSpan(LifeSpan);
}



void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
						 AActor* DamageCauser)
{

	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();

	if (!IsAlive())
	{
		Death();
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	
	if (IsPatrolling())
	{
		StartChasing();
	}
	
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemy::Destroyed()
{
	Super::Destroyed();

	if (EquippedWeapon)
	{
		GetWorld()->SpawnActor<AWeapon>(EquippedWeapon.GetClass(), GetActorLocation(), FRotator::ZeroRotator);
		EquippedWeapon->LifeSpanExpired();
	}
}

