// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"

#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Tresor/Tresor.h"
#include "Kismet/GameplayStatics.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"


// Sets default values
ABreakableActor::ABreakableActor()
{
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometruCollection"));
	SetRootComponent(GeometryCollectionComponent);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GeometryCollectionComponent->SetGenerateOverlapEvents(true);
	GeometryCollectionComponent->bNotifyBreaks = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
	Capsule->SetCollisionResponseToChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}


void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();

	GeometryCollectionComponent->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::ChaosBreakEvent);
}

// cette function est call pour chaque morceau du mesh
void ABreakableActor::ChaosBreakEvent(const FChaosBreakEvent& BreakEvent)
{
	if (!bIsBreak)
	{
		UGameplayStatics::PlaySoundAtLocation(this,PotBreak, GetActorLocation());
		SetLifeSpan(3.f);
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		if (TreasureClasses.Num() > 0)
		{
			FVector Location = GetActorLocation();
			Location.Z += Capsule->GetScaledCapsuleHalfHeight();

			const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
			GetWorld()->SpawnActor<ATresor>(TreasureClasses[Selection], Location, GetActorRotation());
		}

		bIsBreak = true;
	}
}
	


void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint,  AActor* Hitter)
{
	
}


