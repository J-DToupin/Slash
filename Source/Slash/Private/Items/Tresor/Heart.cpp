// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Tresor/Heart.h"

#include "Interfaces/PickUpInterface.h"


AHeart::AHeart()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHeart::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHeart::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor))
	{
		PickUpInterface->AddHeart(this);
		PlaySoundPickup();
		this->Destroy();
	}
}

