// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"


#include "Components/BoxComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTracesStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTracesStart->SetupAttachment(RootComponent);
	BoxTracesEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTracesEnd->SetupAttachment(RootComponent);

	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFallOff"));
	
	FieldSystemMetaDataFilter = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("FieldSystemMetaDataFilter"));
	FieldSystemMetaDataFilter->ObjectType = EFieldObjectType::Field_Object_Destruction;
	
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("RadialVector"));

	MetaData = CreateDefaultSubobject<UFieldSystemMetaData>(TEXT("MetaData"));
	FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystemComponent"));
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnBoxOverlap);
}


void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTracesStart->GetComponentLocation();
	const FVector End = BoxTracesEnd->GetComponentLocation();
	
	IgnoreActors.Add(this);

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTracesStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);
}

void AWeapon::ExecuteGetHit(FHitResult BoxHit)
{
	if (BoxHit.GetActor()->Implements<UHitInterface>())
	{
		IHitInterface::Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}

	IgnoreActors.AddUnique(BoxHit.GetActor());
}

bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("EnemyCharacter")) && OtherActor->ActorHasTag(TEXT("EnemyCharacter"));
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorIsSameType(OtherActor)) return;
	
	if (IgnoreActors.IsEmpty())
	{
		FHitResult BoxHit;
		BoxTrace(BoxHit);

		if (BoxHit.GetActor() && !ActorIsSameType(BoxHit.GetActor()))
		{
			ExecuteGetHit(BoxHit);
			CreateFields(BoxHit.ImpactPoint);

			UGameplayStatics::ApplyDamage(BoxHit.GetActor(), BaseDamage, GetInstigator()->GetController(), this,
			                              UDamageType::StaticClass());
		}
	}
}

void AWeapon::CreateFields(const FVector& FieldLocation)
{
	RadialFalloff->SetRadialFalloff(1000000.f, 0.8f, 1.f, 0.f,200.f, FieldLocation, EFieldFalloffType::Field_FallOff_None);
	RadialVector->SetRadialVector(1500000.f, FieldLocation);
	
	FieldSystemComponent->ApplyPhysicsField(true,EFieldPhysicsType::Field_ExternalClusterStrain, MetaData,RadialFalloff);
	FieldSystemComponent->ApplyPhysicsField(true,EFieldPhysicsType::Field_LinearForce, FieldSystemMetaDataFilter,RadialVector);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules const TransformRules(EAttachmentRule::SnapToTarget,true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::PlaySoundPickup(const bool EnableSound) const
{
	if (EnableSound && EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound,GetActorLocation());
	}
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName,AActor* NewOwner, APawn* NewInstigator,const bool EnableSound)
{
	ItemState = EItemState::Eis_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);

	PlaySoundPickup(EnableSound);

	DisableSphereCollision();
	DisableNiagaraComponent();
}

void AWeapon::UnEquip(const AWeapon* NewWeapon)
{
	GetWorld()->SpawnActor<AWeapon>(GetClass(), NewWeapon->GetActorLocation(),NewWeapon->GetActorRotation());
	Destroy();
}

