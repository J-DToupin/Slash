// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Eis_Unequipped UMETA(DisplayName = "Unequipped"), 
	Eis_Equipped UMETA(DisplayName = "Equipped"),
};

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float RunningTime{};
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, Category=Niagara)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, Category=Niagara)
	TObjectPtr<UNiagaraSystem> PickupNiagaraComponent;

	UPROPERTY(EditAnywhere, Category=Sound)
	TObjectPtr<USoundBase> PickupSound;

protected:

	void PlaySoundPickup(const bool EnableSound = true) const;
	
	void DisableSphereCollision() const;

	void DisableNiagaraComponent() const;

	void SpawnPickupSystem() const;

	UStaticMeshComponent* GetItemMesh() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sine Parameters")
	float Amplitude{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sine Parameters")
	float TimeConstant{};
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	EItemState ItemState = EItemState::Eis_Unequipped;

	template<typename T>
	T Avg(T First, T Second);

public:
	
	virtual void Tick(float DeltaTime) override;
	
};

template <typename T>
T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
