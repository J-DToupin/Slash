// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "AIController.h"
#include "Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute"));

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	HealthBarWidget->SetVisibility(false);
	
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SightRadius = 4000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
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
}

void AEnemy::PLayMontage(const FName& SelectionName, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (!AnimInstance && !Montage) return;
	
	AnimInstance->Montage_Play(Montage);
	AnimInstance->Montage_JumpToSection(SelectionName, Montage);

	if (Montage == DeathMontage)
	{
		DeathSectionName = SelectionName;
		
		if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveMontageInstance())
		{
			MontageInstance->OnMontageBlendingOutStarted.BindUObject(this, &AEnemy::OnDeathMontageBlendingOut);
			
		}
		
	}
	
}

void AEnemy::PlayRandomDeathMontage()
{
	const int32 RandomNum = FMath::RandRange(0,3);
	FName SelectionName;
	switch (RandomNum)
	{
	case 0:
		SelectionName = FName("ShieldDeath");
		break;
	case 1:
		SelectionName = FName("FlyDeath");
		break;
	case 2:
		SelectionName = FName("ForwardDeath");
		break;
	case 3:
		SelectionName = FName("BackDeath");
		break;
	default:
		SelectionName = FName("ShieldDeath");
	}

	PLayMontage(SelectionName, DeathMontage);
}

void AEnemy::OnDeathMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted && Montage == DeathMontage)

	{

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance)

		{

			int32 SectionIndex = DeathMontage->GetSectionIndex(DeathSectionName);

			float SectionStart;

			float SectionEnd;

			DeathMontage->GetSectionStartAndEndTime(SectionIndex, SectionStart, SectionEnd);



			AnimInstance->Montage_Play(DeathMontage);

			AnimInstance->Montage_JumpToSection(DeathSectionName, DeathMontage);

			AnimInstance->Montage_SetPosition(

			DeathMontage,

			SectionStart + DeathMontage->GetSectionLength(SectionIndex) - KINDA_SMALL_NUMBER);



			AnimInstance->Montage_SetPlayRate(DeathMontage, 0.0f);

		}

	}
}

bool AEnemy::InTargetRange(const AActor* Target, const double Radius) const
{
	if (Target == nullptr) return false;
	
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void AEnemy::SetAIMoveToTarget(const AActor* Target) const
{
	// Set AI Enemy
	if (AiController == nullptr || Target == nullptr) return;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);

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
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}

		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		SetAIMoveToTarget(PatrolTarget);
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		SetAIMoveToTarget(CombatTarget);
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Attacking;
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
	if (EnemyState == EEnemyState::EES_Chasing) return;
	
	if (Target->ActorHasTag(FName("SlashCharacter")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = Target;

		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			SetAIMoveToTarget(Target);
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
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
	
	// DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Blue, true);
	// DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, true);
	// DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, 5.f, FColor::Yellow, true);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
		
	}

	//DRAW_SPHERE(ImpactPoint, FColor::Red)
	if (Attribute->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticle)
	{
		//version cascade
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),HitParticle, ImpactPoint);
	}

	
}

void AEnemy::Death()
{
	PlayRandomDeathMontage();
	HealthBarWidget->DestroyComponent();
	GetCapsuleComponent()->DestroyComponent();
	SetLifeSpan(3.f);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	if (Attribute && HealthBarWidget)
	{
		Attribute->AddHealth(-DamageAmount);
		
		HealthBarWidget->SetHealthPercent(Attribute->GetPercentHealth());

		if (!Attribute->IsAlive())
		{
			Death();
		}
	}

	CombatTarget = EventInstigator->GetPawn();

	if (EnemyState == EEnemyState::EES_Patrolling)
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		SetAIMoveToTarget(CombatTarget);
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

