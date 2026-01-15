
// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapons/Weapon.h"
#include "Characters/CPPCharacter.h"

#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

#include "Interfaces/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
    // --- Root & Scale Container --------------------------------------------
    // If a parent class (AItem) already made a root (often the ItemMesh), we still
    // want a dedicated ScaleContainer as the *new* root so we can scale everything
    // uniformly. We reattach the previous root under ScaleContainer.
    USceneComponent* PreviousRoot = RootComponent;

    ScaleContainer = CreateDefaultSubobject<USceneComponent>(TEXT("ScaleContainer"));

    if (PreviousRoot)
    {
        // Reparent previous root under the new ScaleContainer; keep its relative transform
        PreviousRoot->SetupAttachment(ScaleContainer);
    }

    // Make ScaleContainer the root so SetRelativeScale3D on it affects the whole weapon.
    RootComponent = ScaleContainer;

    // --- Class-owned Components --------------------------------------------
    WeaponBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Collider"));
    WeaponBoxComponent->SetupAttachment(ScaleContainer);
    WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    WeaponBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
    BoxTraceStart->SetupAttachment(ScaleContainer);

    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
    BoxTraceEnd->SetupAttachment(ScaleContainer);
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    EnsurePartsAttachedToScaleContainer();
}

void AWeapon::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    EnsurePartsAttachedToScaleContainer();
}



#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

void AWeapon::EnsurePartsAttachedToScaleContainer()
{
    if (!ScaleContainer) return;

    // WeaponBoxComponent
    if (WeaponBoxComponent && WeaponBoxComponent->GetAttachParent() != ScaleContainer)
    {
        WeaponBoxComponent->AttachToComponent(ScaleContainer, FAttachmentTransformRules::KeepRelativeTransform);
    }

    // BoxTraceStart
    if (BoxTraceStart && BoxTraceStart->GetAttachParent() != ScaleContainer)
    {
        BoxTraceStart->AttachToComponent(ScaleContainer, FAttachmentTransformRules::KeepRelativeTransform);
    }

    // BoxTraceEnd
    if (BoxTraceEnd && BoxTraceEnd->GetAttachParent() != ScaleContainer)
    {
        BoxTraceEnd->AttachToComponent(ScaleContainer, FAttachmentTransformRules::KeepRelativeTransform);
    }

    // ItemMesh (inherited from AItem)
    if (USceneComponent* ItemMeshScene = Cast<USceneComponent>(ItemMesh))
    {
        if (ItemMeshScene->GetAttachParent() != ScaleContainer)
        {
            ItemMeshScene->AttachToComponent(ScaleContainer, FAttachmentTransformRules::KeepRelativeTransform);
        }
    }

    // sphereCollider (inherited from AItem)
    if (USceneComponent* SphereScene = Cast<USceneComponent>(sphereCollider))
    {
        if (SphereScene->GetAttachParent() != ScaleContainer)
        {
            SphereScene->AttachToComponent(ScaleContainer, FAttachmentTransformRules::KeepRelativeTransform);
        }
    }
}


/**
 * Attach the entire weapon hierarchy (ScaleContainer root) to the target socket.
 * This ensures scaling via ScaleContainer continues to affect the mesh & colliders after equip.
 */
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{

    UE_LOG(LogTemp, Log, TEXT("AWeapon::Equip called on %s, Owner=%s, Instigator=%s"),
        *GetName(), *GetNameSafe(GetOwner()), *GetNameSafe(GetInstigator()));

    SetOwner(NewOwner);
    SetInstigator(NewInstigator);

    const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);

    // Attach the *root* (ScaleContainer) so the whole hierarchy moves with the socket
    if (ScaleContainer && InParent)
    {
        ScaleContainer->AttachToComponent(InParent, TransformRules, InSocketName);
    }

    // Maintain your existing behavior for the sphere collider on equip
    if (sphereCollider)
    {
        sphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (WeaponBoxComponent)
    {
        WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
    }
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnEndSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Use the trace anchors for start/end
    const FVector Start = BoxTraceStart ? BoxTraceStart->GetComponentLocation() : FVector::ZeroVector;
    const FVector End = BoxTraceEnd ? BoxTraceEnd->GetComponentLocation() : FVector::ZeroVector;

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    for (AActor* Actor : ignoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }

    FHitResult BoxHit;

    // If you want the trace "thickness" to scale with weapon size, consider multiplying the extents
    // by the ScaleContainer's scale or compute based on the distance between Start and End.
    const FVector HalfSize(5.f, 5.f, 5.f);

    UKismetSystemLibrary::BoxTraceSingle(
        this,
        Start,
        End,
        HalfSize,
        BoxTraceStart ? BoxTraceStart->GetComponentRotation() : FRotator::ZeroRotator,
        ETraceTypeQuery::TraceTypeQuery1,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForDuration,
        BoxHit,
        true
    );

    if (AActor* HitActor = BoxHit.GetActor())
    {
        // Apply damage
        UGameplayStatics::ApplyDamage(
            HitActor,
            damage,
            GetInstigator() ? GetInstigator()->GetController() : nullptr,
            this,
            UDamageType::StaticClass()
        );

        // Post-damage hit reaction
        if (IHitInterface* HitInterface = Cast<IHitInterface>(HitActor))
        {
            HitInterface->GetHit(BoxHit.ImpactPoint);
        }

        ignoreActors.AddUnique(HitActor);

        if (HitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, HitSound, BoxHit.ImpactPoint, 1.f, 1.f, 0.1f);
        }
    }
    else
    {
        if (MissSoundEffect)
        {
            UGameplayStatics::PlaySoundAtLocation(this, MissSoundEffect, GetActorLocation(), 1.f, 1.f, 0.1f);
        }
    }
}

/** Uniformly scale the whole weapon by scaling the ScaleContainer (root) */
void AWeapon::ScaleWeapon(float ScaleFactor)
{
    if (!ScaleContainer) return;

    // Prefer uniform scale for stable physics/collision
    const FVector NewScale(ScaleFactor);
    ScaleContainer->SetRelativeScale3D(NewScale);
}

/**
 * Optional: fine-grained scaling—resizes specific parts explicitly.
 * NOTE: If you call this repeatedly, consider caching base values (radius, extents, relative locations)
 * once in OnConstruction/BeginPlay to avoid compounding transforms on the trace anchors.
 */
void AWeapon::ScaleWeaponExplicit(float ScaleFactor)
{
    // Visible mesh (if you want to scale visual size independently)
    if (ItemMesh)
    {
        ItemMesh->SetRelativeScale3D(FVector(ScaleFactor));
    }

    // Sphere collision – adjust the unscaled radius (more accurate than scaling the component)
    if (sphereCollider)
    {
        const float BaseRadius = sphereCollider->GetUnscaledSphereRadius();
        sphereCollider->SetSphereRadius(BaseRadius * ScaleFactor, /*bUpdateOverlaps=*/true);
    }

    // Box collision – adjust unscaled extents
    if (WeaponBoxComponent)
    {
        const FVector BaseExt = WeaponBoxComponent->GetUnscaledBoxExtent();
        WeaponBoxComponent->SetBoxExtent(BaseExt * ScaleFactor, /*bUpdateOverlaps=*/true);
    }

    // Trace anchors – move proportionally (scales trace length)
    if (BoxTraceStart)
    {
        const FVector BaseStart = BoxTraceStart->GetRelativeLocation();
        BoxTraceStart->SetRelativeLocation(BaseStart * ScaleFactor);
    }
    if (BoxTraceEnd)
    {
        const FVector BaseEnd = BoxTraceEnd->GetRelativeLocation();
        BoxTraceEnd->SetRelativeLocation(BaseEnd * ScaleFactor);
    }
}
