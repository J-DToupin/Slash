// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Heart.generated.h"

UCLASS()
class SLASH_API AHeart : public AItem
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	int32 HealthCount{35};
	
	AHeart();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	float GetHealthCount() const
	{
		return HealthCount;
	}

	
};
