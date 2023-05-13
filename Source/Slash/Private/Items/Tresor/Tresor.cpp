// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Tresor/Tresor.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATresor::ATresor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATresor::BeginPlay()
{
	Super::BeginPlay();
	
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
		
		this->Destroy();
	}
	
}

// Called every frame
void ATresor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.f,2.f,0.f));
}

