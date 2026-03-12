
// Weapon.h
#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h" // Base class providing ItemMesh/sphereCollider & overlap hooks
#include "Weapon.generated.h"

class UBoxComponent;
class USceneComponent;
class UTexture2D;
class USoundBase;
class UWeaponDataAsset;
class UPrimitiveComponent;
class APawn;

/** Events broadcast in Weapon.cpp */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquipped, bool, bEquipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponPurchased, bool, bPurchased);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponUpgraded, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponUnequipped, bool, bEquipped);

UCLASS()
class CRAZYARENACHAOS_API AWeapon : public AItem
{
    GENERATED_BODY()

public:
    AWeapon();

    // ===== Lifecycle =====
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;

    // ===== Initialization from Data Asset (ONE-TIME CONFIG COPY) =====
    /** Initializes this instance from an immutable config. Call once per instance. */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Init")
    void InitializeFromDataAsset(UWeaponDataAsset* InDataAsset);

    // ===== Per-instance identity =====
    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Identity")
    FGuid InstanceGuid;

    /** Optional human-authored per-instance identity (from placeholder) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Identity")
    FName DesignerInstanceKey = NAME_None;

    /** Returns a string key you can use in Blueprint Maps (e.g., "26F1C1C7-...") */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Identity")
    FString GetInstanceIdString() const; // implementation in .cpp

    /** Ensures InstanceGuid is valid; generates one if missing (returns final value) */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Identity")
    FGuid EnsureInstanceGuid(); // (implementation in .cpp)

    /** Primary shelf key = DesignerInstanceKey (if set) else InstanceGuid string */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Identity")
    FString GetPrimaryShelfKey() const { return !DesignerInstanceKey.IsNone() ? DesignerInstanceKey.ToString() : GetInstanceIdString(); }

    /**
     * Self-contained definition identity (type-level, not per-instance).
     * Prefers StableGuid if authored; else StableName; else class name.
     * Does NOT require DataAsset to be present.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Identity")
    FGuid GetStableGuid() const;

    /** Ensures StableName has a sensible fallback when no DataAsset is used. */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Identity")
    void EnsureStableDefinitionIdentity();

    // ===== Attach / Equip =====
    /** Attaches the weapon (ScaleContainer root) to a socket. No re-initialization. */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

    /** Explicit toggle so external code (Character/Shop) can keep state & events consistent. */
    UFUNCTION(BlueprintCallable, Category = "Weapon|State")
    void SetEquipped(bool bInEquipped);


    /** Event-driven Unequip (does not hide by default; Shop/Shelf handles parking). */
    UFUNCTION(BlueprintCallable, Category = "Weapon|State")
    void Unequip(bool bReturnToInventory = true, USceneComponent* InventoryParent = nullptr, FName InventorySocket = NAME_None);

    // ===== Economy / Leveling actions (method-driven mutators) =====
    UFUNCTION(BlueprintCallable, Category = "Weapon|Economy")
    void BuyWeapon();

    UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
    void UpgradeWeapon();

    // ===== Read-only getters for Blueprint logic (no generic setters) =====
    UFUNCTION(BlueprintPure, Category = "Weapon|State")
    bool IsEquipped() const { return bIsEquipped; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Economy")
    bool IsPurchased() const { return bPurchased; }

    /** Current (level-scaled) price using your formula. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Config", meta = (AllowPrivateAccess = "true"))
    float  KnockBackAmount = 10.0f;
    UFUNCTION(BlueprintPure, Category = "Weapon|Economy")
    int32 GetPrice() const;

    UFUNCTION(BlueprintPure, Category = "Weapon|Level")
    bool CanUpgrade() const;

    UFUNCTION(BlueprintPure, Category = "Weapon|Level")
    int32 GetCurrentLevel() const { return CurrentLevel; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Level")
    int32 GetMaxLevel() const { return MaxLevel; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Damage")
    float GetBaseDamage() const { return BaseDamageCached; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Damage")
    float GetDamageScaling() const { return DamageScaling; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Visual")
    float GetSizeScaleFactor() const { return SizeScaleFactor; }

    UFUNCTION(BlueprintPure, Category = "Weapon|UI")
    UTexture2D* GetIcon() const { return Icon; }

    /** Accessor used in anim notifies / gameplay to gate collisions */
    UFUNCTION(BlueprintPure, Category = "Weapon|Collision")
    UBoxComponent* GetWeaponBoxComponent() const { return WeaponBoxComponent; }

    // ===== Visual scaling utilities =====
    /** Uniformly scales the whole hierarchy (via ScaleContainer). */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Visual")
    void ScaleWeapon(float ScaleFactor);

    /** Optional fine-grained scaling of individual parts. */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Visual")
    void ScaleWeaponExplicit(float ScaleFactor);

    // ===== Events =====
    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnWeaponEquipped OnEquipped;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnWeaponPurchased OnPurchased;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnWeaponUpgraded OnUpgraded;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnWeaponUnequipped OnUnequipped;

    // ===== Exposed for Character montage collision gate =====
    // Your Character clears this on collision enable:
    // equippedWeapon->ignoreActors.Empty();
    UPROPERTY()
    TArray<AActor*> ignoreActors;

protected:
    // Overlap hooks (AItem provides UFUNCTIONs; DO NOT repeat UFUNCTION() here)
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult) override;

    virtual void OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

    // Needs UFUNCTION because we bind it with AddDynamic in BeginPlay
    UFUNCTION()
    void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

private:
    // ===== Internal helpers =====
    void EnsurePartsAttachedToScaleContainer();
    void ApplyRuntimeScale();

private:
    // ===== Components =====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* ScaleContainer = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* WeaponBoxComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* BoxTraceStart = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* BoxTraceEnd = nullptr;

    // ===== Runtime state =====
    UPROPERTY(BlueprintReadOnly, Category = "Weapon|State", meta = (AllowPrivateAccess = "true"))
    bool bIsEquipped = false;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon|State", meta = (AllowPrivateAccess = "true"))
    bool bPurchased = false;

    // ===== Audio =====
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Audio", meta = (AllowPrivateAccess = "true"))
    USoundBase* HitSound = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Audio", meta = (AllowPrivateAccess = "true"))
    USoundBase* MissSoundEffect = nullptr;

    // ===== Config copied from Data Asset (immutable origin, mutable here) =====

    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Config", meta = (AllowPrivateAccess = "true"))
    float  BaseDamageCached = 10.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Config", meta = (AllowPrivateAccess = "true"))
    float  DamageScaling = 1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Config", meta = (AllowPrivateAccess = "true"))
    int32  Price = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Config", meta = (AllowPrivateAccess = "true"))
    float  PriceScaling = 1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Config", meta = (AllowPrivateAccess = "true"))
    int32  MaxLevel = 5;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Config", meta = (AllowPrivateAccess = "true"))
    int32  CurrentLevel = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Config", meta = (AllowPrivateAccess = "true"))
    float  SizeScaleFactor = 0.0f;

    // Current computed damage (used in ApplyDamage)
    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Runtime", meta = (AllowPrivateAccess = "true"))
    float damage = 0.0f;

    // Optional UI asset (use if you wire it later)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|UI", meta = (AllowPrivateAccess = "true"))
    UTexture2D* Icon = nullptr;

public:
    // Reference back to the source config (handy for debug). Not required for identity.
    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Config", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UWeaponDataAsset> DataAsset = nullptr;

    // ===== Definition identity & display (copied from UWeaponDataAsset or set by designer) =====
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Definition", meta = (AllowPrivateAccess = "true"))
    FName  StableName = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Definition", meta = (AllowPrivateAccess = "true"))
    FName  DesignerListKey = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Definition", meta = (AllowPrivateAccess = "true"))
    FGuid  StableGuid;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Definition", meta = (AllowPrivateAccess = "true"))
    FString WeaponDisplayName;
};
