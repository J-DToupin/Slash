// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Tresor/Tresor.h"
#include "Interfaces/PickUpInterface.h"


// Sets default values
ATresor::ATresor()
{
	
}

// Called when the game starts or when spawned
void ATresor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TresorTimerHandle, this, &ATresor::TresorRotation, 0.01f, true);
}

void ATresor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor))
	{
		SelectType(PickUpInterface);
		PlaySoundPickup();
		GetWorldTimerManager().ClearTimer(TresorTimerHandle);
		this->Destroy();
	}
	
}

void ATresor::SelectType(IPickUpInterface* PickUpInterface)
{
	switch (TreasureType)
	{
	case ETreasureType::ETT_Gold:
		PickUpInterface->AddGold(this);
		break;
	case ETreasureType::ETT_Health:
		PickUpInterface->AddHealth(this);
		break;

		default:
			break;
	}
}

void ATresor::TresorRotation()
{
	AddActorWorldRotation(FRotator(0.f,2.f,0.f));
}

int32 ATresor::GetValue() const
{
	return Value;
}
