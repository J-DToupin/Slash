// Fill out your copyright notice in the Description page of Project Settings.



#include "Slash/Public/Items/Item.h"
#include "Slash/DebugMacros.h"


// Sets default values
AItem::AItem() : Amplitude(0.25f), TimeConstant(5.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	int32 AvgInt = Avg<int32>(1, 3);
	UE_LOG(LogTemp, Warning, TEXT("ABG of 1 and 3 ; %d"), AvgInt)

	UE_LOG(LogTemp,Warning, TEXT("Begin Play Called!"))

	

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f,FColor::Orange,TEXT("Item OnScreen Message!"));
	}

	SetActorLocation(FVector(0.f,0.f, 50.f));

	FVector Location{GetActorLocation()};
	FVector Forward{GetActorForwardVector()};
	
	// Draw une sphere  avec une macro dans DebugMacros.h
	DRAW_SPHERE(Location, FColor::Red)
	// DRAW_LINE(Location, Location + Forward * 100.f)
	// DRAW_POINT(Location + Forward * 100.f)
	DRAW_VECTOR(Location, Location + Forward * 100.f, FColor::Blue)
	
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement rate in units of cm/s
	float MovementRate = 50.f;
	float RotationRate = 45.f;

	// MovemntRate * Deltatime (cm/s) * (s/frame) = (cm/frame)
	AddActorWorldOffset(FVector(MovementRate * DeltaTime,0.f,0.f));
	AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));

	RunningTime += DeltaTime;
	//float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);

	//AddActorWorldOffset(FVector(0.f,0.f, DeltaZ));
	
	// DRAW_SPHERE_FRAME(GetActorLocation(),FColor::Green)
	// DRAW_VECTOR_FRAME(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100, FColor::Blue)

	// FString const Name{GetName()};
	//
	// GEngine->AddOnScreenDebugMessage(1, 60.f,FColor::Orange,FString::Printf(TEXT("Item OnScreen Message! %f"), DeltaTime));
	// GEngine->AddOnScreenDebugMessage(1, 60.f,FColor::Orange,FString::Printf(TEXT("Name = %s"), *Name));

}

