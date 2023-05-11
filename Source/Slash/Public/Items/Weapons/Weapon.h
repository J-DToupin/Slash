// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
class USoundBase;
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();
	
private:

	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	TObjectPtr<UBoxComponent> WeaponBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTracesStart;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTracesEnd;

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

public:

	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);

	void Equip(USceneComponent* InParent, FName InSocketName);

	FORCEINLINE UBoxComponent* GetWeaponBox() const
	{
		return WeaponBox;
	}
};
