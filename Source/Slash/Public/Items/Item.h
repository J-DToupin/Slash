// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
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
	USphereComponent* SphereComponent;


	

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

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
