// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Characters/Player/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"



// Sets default values
AItem::AItem() : Amplitude(0.25f), TimeConstant(5.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(GetRootComponent());

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEmbers"));
	NiagaraComponent->SetupAttachment(GetRootComponent());
	

}

void AItem::DisableSphereCollision() const
{
	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AItem::DisableNiagaraComponent() const
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor))
	{
		SlashCharacter->SetOverlappingItem(this);
	}

	GEngine->AddOnScreenDebugMessage(1,30.f, FColor::Red, TEXT("In"));
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor))
	{
		SlashCharacter->SetOverlappingItem(nullptr);
	}

	GEngine->AddOnScreenDebugMessage(1,30.f, FColor::Red, TEXT("Out"));
}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement rate in units of cm/s
	RunningTime += DeltaTime;
	float MovementRate = 50.f;
	float RotationRate = 45.f;

	if (ItemState == EItemState::Eis_Unequipped)
	{
		AddActorWorldOffset(FVector(0.f,0.f, TransformedSin()));
	}


	// MovemntRate * Deltatime (cm/s) * (s/frame) = (cm/frame)
	//AddActorWorldOffset(FVector(MovementRate * DeltaTime,0.f,0.f));
	//AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));

	//float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);


	// DRAW_SPHERE_FRAME(GetActorLocation(),FColor::Green)
	// DRAW_VECTOR_FRAME(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100, FColor::Blue)

	// FString const Name{GetName()};
	//
	// GEngine->AddOnScreenDebugMessage(1, 60.f,FColor::Orange,FString::Printf(TEXT("Item OnScreen Message! %f"), DeltaTime));
	// GEngine->AddOnScreenDebugMessage(1, 60.f,FColor::Orange,FString::Printf(TEXT("Name = %s"), *Name));

}

