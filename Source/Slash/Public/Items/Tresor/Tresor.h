// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Tresor.generated.h"

UCLASS()
class SLASH_API ATresor : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATresor();

private:

	UPROPERTY(EditAnywhere, Category=Sound)
	TObjectPtr<USoundWave> PickUpSound;

	UPROPERTY(EditAnywhere, Category="Treasure Properties")
	int32 Gold{};

	UPROPERTY()
	FTimerHandle TresorTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void TresorRotation();


};
