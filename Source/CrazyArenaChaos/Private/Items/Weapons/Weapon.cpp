// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/CPPCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	WeaponBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Collider"));
	WeaponBoxComponent->SetupAttachment(GetRootComponent());
	WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart=CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd=CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	sphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndSphereOverlap( OverlappedComponent,OtherActor,OtherComp,OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector start = BoxTraceStart->GetComponentLocation();
	const FVector end = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FHitResult boxHit;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		start,
		end,
		FVector(5.f,5.f,5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		boxHit,
		true
		);
	if (boxHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(boxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(boxHit.ImpactPoint);
		}
	}
}
