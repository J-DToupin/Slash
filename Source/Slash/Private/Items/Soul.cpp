// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"

#include "Interfaces/PickUpInterface.h"


ASoul::ASoul()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ASoul::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);

	if (PickUpInterface)
	{
		PickUpInterface->AddSouls(this);
		Destroy();
	}
}


void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

