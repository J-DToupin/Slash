// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Tresor.generated.h"


class IPickUpInterface;

UENUM(BlueprintType)
enum class ETreasureType : uint8
{
	ETT_Gold UMETA(DisplayName = "Give Gold"), 
	ETT_Health UMETA(DisplayName = "Give Heath"),
	ETT_Stamina UMETA(DisplayName = "Give Stamina")
};

UCLASS()
class SLASH_API ATresor : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATresor();

private:
	
	UPROPERTY(EditAnywhere, Category="Treasure Properties")
	int32 Value{};

	UPROPERTY()
	FTimerHandle TresorTimerHandle;

	UPROPERTY(EditAnywhere)
	ETreasureType TreasureType{ETreasureType::ETT_Gold};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void SelectType(IPickUpInterface* PickUpInterface);

	void TresorRotation();

public:
	int32 GetValue() const;


};
