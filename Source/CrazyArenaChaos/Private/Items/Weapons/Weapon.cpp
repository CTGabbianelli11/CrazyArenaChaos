
// Weapon.cpp
#include "Items/Weapons/Weapon.h"
#include "Characters/CPPCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/WeaponDataAsset.h"
#include "Components/AC_HitStop.h"

AWeapon::AWeapon()
{
    // --- Root & Scale Container ---
    // If a parent class (AItem) already made a root (often the ItemMesh),
    // we still want a dedicated ScaleContainer as the *new* root so we can scale everything uniformly.
    // We reattach the previous root under ScaleContainer.
    USceneComponent* PreviousRoot = RootComponent;
    ScaleContainer = CreateDefaultSubobject<USceneComponent>(TEXT("ScaleContainer"));
    if (PreviousRoot)
    {
        // Reparent previous root under the new ScaleContainer; keep its relative transform
        PreviousRoot->SetupAttachment(ScaleContainer);
    }
    // Make ScaleContainer the root so SetRelativeScale3D on it affects the whole weapon.
    RootComponent = ScaleContainer;

    // --- Class-owned Components ---
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

    // Avoid touching the CDO
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        EnsureInstanceGuid();
        EnsureStableDefinitionIdentity(); // Ensure StableName fallback even without DA
    }
}

FGuid AWeapon::EnsureInstanceGuid()
{
    if (!InstanceGuid.IsValid())
    {
        InstanceGuid = FGuid::NewGuid();
    }
    return InstanceGuid;
}

FString AWeapon::GetInstanceIdString() const
{
    return InstanceGuid.IsValid()
        ? InstanceGuid.ToString(EGuidFormats::DigitsWithHyphens)
        : FString();
}

void AWeapon::ApplyRuntimeScale()
{
    // Preserve original intent: 1.0 + (SizeScaleFactor * CurrentLevel)
    const float NewScale = 1.0f + (SizeScaleFactor * CurrentLevel);
    const float Clamped = FMath::Max(NewScale, 0.01f);
    // If you want invariance to the parent’s scale, use SetWorldScale3D on the root.
    // Otherwise, keep relative scaling (current behavior).
    ScaleWeapon(Clamped);
}

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

    // ---- Reverse what Unequip did: actor is visible & globally collidable again ----
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    // Maintain existing behavior: sphere off in-hand; hit box gated by anim notifies
    if (sphereCollider)
    {
        sphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    if (WeaponBoxComponent)
    {
        WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        ignoreActors.Empty();
    }

    // ---- Runtime state + event ----
    bIsEquipped = true;
    ApplyRuntimeScale();

    // Broadcast that this weapon is now equipped (for UI or gameplay listeners).
    OnEquipped.Broadcast(bIsEquipped);
}

void AWeapon::Unequip(bool /*bReturnToInventory*/, USceneComponent* /*InventoryParent*/, FName /*InventorySocket*/)
{
    // Flip equipped state and notify listeners (Shop shelf will listen to this)
    bIsEquipped = false;
    OnEquipped.Broadcast(false);

    // Turn off combat collisions/traces
    if (WeaponBoxComponent)
    {
        WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        ignoreActors.Empty();
    }
    if (sphereCollider)
    {
        sphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // Detach from the character; keep world so Shop can decide where to move it
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    // Keep visible for tiles by default; disable interaction on shelf
    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
    // Clear ownership so damage routing/instigator aren’t attributed to the player anymore
    SetOwner(nullptr);
    SetInstigator(nullptr);
    OnUnequipped.Broadcast(bIsEquipped);

}

void AWeapon::SetEquipped(bool bInEquipped)
{
    if (bIsEquipped == bInEquipped) return;
    bIsEquipped = bInEquipped;
    OnEquipped.Broadcast(bIsEquipped);

}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();
    EnsureInstanceGuid();

    if (WeaponBoxComponent)
    {
        WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
    }
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnEndSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
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
        EDrawDebugTrace::None,
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

        if (Cast<ACPPCharacter>(GetOwner()))
        {
            Cast<ACPPCharacter>(GetOwner())->hitStopComponent->BeginHitStop(.15f,.05,30,1);
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
 * Optional:
 * fine-grained scaling—resizes specific parts explicitly.
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

// ---- New: definition identity is self-contained (no DA required) ----
void AWeapon::EnsureStableDefinitionIdentity()
{
    // If a designer didn’t set a StableName (e.g., hand-placed instance without DA),
    // fall back to something deterministic but local: the class name.
    if (StableName.IsNone())
    {
        StableName = GetClass() ? GetClass()->GetFName() : FName(TEXT("Weapon"));
    }
}

FGuid AWeapon::GetStableGuid() const
{
    return StableGuid;
}

/** ---- Initialization & runtime that used to be on the Data Asset ---- */
void AWeapon::InitializeFromDataAsset(UWeaponDataAsset* InDataAsset)
{
    EnsureInstanceGuid();
    DataAsset = InDataAsset;
    if (!DataAsset) return;

    // Copy config locally (immutable at runtime)
    BaseDamageCached = DataAsset->BaseDamage;
    DamageScaling = DataAsset->DamageScaling;
    Price = DataAsset->Price;
    PriceScaling = DataAsset->PriceScaling;
    MaxLevel = DataAsset->MaxLevel;
    SizeScaleFactor = DataAsset->SizeScaleFactor;
    Icon = DataAsset->Icon;

    // ---- Definition identity & display (copied from DA) ----
    StableName = DataAsset->StableName;
    DesignerListKey = DataAsset->DesignerListKey; // requires this field on DA
    StableGuid = DataAsset->StableGuid;
    WeaponDisplayName = DataAsset->WeaponName;

    // ---- Defaults coming "from the data object" ----
    bPurchased = DataAsset->bPurchased;
    if (bIsEquipped) { OnEquipped.Broadcast(true); }
    if (bPurchased) { OnPurchased.Broadcast(true); }

    // Set initial runtime values
    CurrentLevel = 1;
    // Preserve original formula: BaseDamage * DamageScaling * Level
    damage = BaseDamageCached * DamageScaling * static_cast<float>(CurrentLevel); // == 0 at level 0
    ApplyRuntimeScale();

    // Make sure we still have a sane StableName if DA fields were empty
    EnsureStableDefinitionIdentity();
}

void AWeapon::BuyWeapon()
{
    bPurchased = true;
    OnPurchased.Broadcast(bPurchased);
}

bool AWeapon::CanUpgrade() const
{
    return CurrentLevel < MaxLevel;
}

void AWeapon::UpgradeWeapon()
{
    if (!CanUpgrade()) return;

    CurrentLevel = FMath::Clamp(CurrentLevel + 1, 0, MaxLevel);
    // Preserve original damage model: BaseDamage * DamageScaling * Level
    damage = BaseDamageCached * DamageScaling * static_cast<float>(CurrentLevel);
    ApplyRuntimeScale();

    // Notify listeners (UI, store, analytics, etc.)
    OnUpgraded.Broadcast(CurrentLevel);
}

int32 AWeapon::GetPrice() const
{
    // Preserve original price model: Price * PriceScaling * Level
    return static_cast<int32>(Price * PriceScaling * static_cast<float>(CurrentLevel));
}

