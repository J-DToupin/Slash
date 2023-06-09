﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Soul.generated.h"

class UNiagaraSystem;

UCLASS()
class SLASH_API ASoul : public AItem
{
	GENERATED_BODY()

private:
	int32 SoulCount{1};



	ASoul();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	
	int32 GetSoulCount() const
	{
		return SoulCount;
	}

	void SetSoulCount(int32 NewSoulCount)
	{
		this->SoulCount = NewSoulCount;
	}
	
};
