
// WeaponDataAsset.h  (config-only; project-local identity)
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h" 

class AWeapon;

/**
 * Immutable configuration for a weapon.
 * No runtime/mutable state or events; those live on AWeapon.
 * Project-local identity is provided by StableName (+ optional StableGuid).
 */
UCLASS(Blueprintable, BlueprintType)
class CRAZYARENACHAOS_API UWeaponDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    /** ---------- Identity / Type (config) ---------- */

    /** Designer-visible, stable name for this weapon (defaulted to asset name if left None) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Identity")
    FName StableName = NAME_None;

    //* Designer name to use in setting up different scenes */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Designer")
    FName DesignerListKey = NAME_None; // e.g., "Katana"

    /** Optional global GUID for external systems / migrations (auto-filled once if empty) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Identity")
    FGuid StableGuid;

    /** Human-readable label for UI (not the identifier) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FString WeaponName = TEXT("Generic Weapon");

    /** Concrete Weapon class to spawn when creating an instance */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> WeaponToEquip;

    /** ---------- Economy (config) ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Economy", meta = (ClampMin = "0"))
    int32 Price = 0;
    
    //* Scaling factor for pricing.  The calculated value is price + Price * CurrentLevel
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Economy", meta = (ClampMin = "0.0"))
    float PriceScaling = 1.0f;

    /** ---------- Leveling (config) ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Level", meta = (ClampMin = "0"))
    int32 MaxLevel = 5;

    /** ---------- Damage (config) ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage", meta = (ClampMin = "0.0"))
    float BaseDamage = 10.0f;

    //* Damage scalar.  Value is calculated as Damage + Damage * CurrentLevel
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage", meta = (ClampMin = "0.0"))
    float DamageScaling = 1.0f;

    /** Visual scale factor applied per level (used by weapon logic).  Calculated as Size + SizeScaleFactor * CurrentLevel.  A value of 1 would double the size. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
    float SizeScaleFactor = 0.0f;

    /** ---------- UI (config) ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|UI")
    UTexture2D* Icon = nullptr;


    // In WeaponDataAsset.h (config-only defaults, not runtime state)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Defaults")
    bool bPurchased = false;

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Identity")
    FGuid GetStableGUID() const;


    virtual void PostLoad() override;

private:
    void EnsureStableIdentity();
};
