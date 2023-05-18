// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "AIController.h"
#include "Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"

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
}

void AEnemy::SetAI() const
{
	// Set AI Enemy
	if (AiController && PatrolTarget)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);

		// FNavPathSharedPtr pointer qui store les data de Moveto
		AiController->MoveTo(MoveRequest);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AiController = Cast<AAIController>(GetController());
	SetAI();
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
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		
		if (!InTargetRange(CombatTarget, CombatRadius))
		{
			CombatTarget = nullptr;
			if (HealthBarWidget)
			{
				HealthBarWidget->SetVisibility(false);
			}
		}
	}

	if (PatrolTarget)
	{
		if (InTargetRange(PatrolTarget, PatrolRadius))
		{
			if (!PatrolTargets.IsEmpty())
			{
				const int32 TargetSelection = FMath::RandRange(0, PatrolTargets.Num() - 1);

				PatrolTarget = PatrolTargets[TargetSelection];
				SetAI();
			}
		}
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
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

