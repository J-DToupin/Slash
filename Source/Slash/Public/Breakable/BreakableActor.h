// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

struct FChaosBreakEvent;
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

	bool bIsBreak{};

	UPROPERTY(EditAnywhere, Category=Sound)
	TObjectPtr<USoundWave> PotBreak;

	//TSubclass fait un UClass qui peut etre seulement etre un ATresor
	UPROPERTY(EditAnywhere, Category=Class)
	TArray<TSubclassOf<ATresor>> TreasureClasses;
	

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Capsule;
	
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ChaosBreakEvent(const FChaosBreakEvent& BreakEvent);


public:

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	
};
