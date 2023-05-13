﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UCapsuleComponent;
class ATresor;
class FOnChaosBreakEvent;
class UGeometryCollectionComponent;
class USoundWave;
UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()

public:
	ABreakableActor();

private:
	

	UPROPERTY(EditAnywhere, Category=Sound)
	TObjectPtr<USoundWave> PotBreak;

	//TSubclass fait un UClass qui peut etre seulement etre un ATresor
	UPROPERTY(EditAnywhere, Category=Class)
	TSubclassOf<ATresor> TreasureClass;
	

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Capsule;
	
	virtual void BeginPlay() override;

public:

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	
};
