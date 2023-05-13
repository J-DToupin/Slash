// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"

#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Tresor/Tresor.h"
#include "Kismet/GameplayStatics.h"


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
	
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	UGameplayStatics::PlaySoundAtLocation(this,PotBreak, ImpactPoint);

	if (TreasureClass)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		GetWorld()->SpawnActor<ATresor>(TreasureClass, Location, GetActorRotation());
	}
	//SetLifeSpan(3.f);
}


