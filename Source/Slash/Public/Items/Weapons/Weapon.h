// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class URadialVector;
class UFieldSystemMetaDataFilter;
class URadialFalloff;
class UBoxComponent;
class USoundBase;
class UFieldSystemMetaData;
class UFieldSystemComponent;
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

	UPROPERTY(EditAnywhere)
	TObjectPtr<URadialFalloff> RadialFalloff;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UFieldSystemMetaDataFilter> FieldSystemMetaDataFilter;
	UPROPERTY(EditAnywhere)
	TObjectPtr<URadialVector> RadialVector;

	UPROPERTY()
	TObjectPtr<UFieldSystemMetaData> MetaData;

	UPROPERTY()
	TObjectPtr<UFieldSystemComponent> FieldSystemComponent;

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CreateFields(const FVector& FieldLocation);

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
