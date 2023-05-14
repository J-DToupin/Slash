// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Tresor/Tresor.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"


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
	if (ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor))
	{
		if (PickUpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
		}
		GetWorldTimerManager().ClearTimer(TresorTimerHandle);
		this->Destroy();
	}
	
}

void ATresor::TresorRotation()
{
	AddActorWorldRotation(FRotator(0.f,2.f,0.f));
}
