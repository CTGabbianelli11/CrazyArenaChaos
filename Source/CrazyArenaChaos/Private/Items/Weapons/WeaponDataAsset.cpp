
// WeaponDataAsset.cpp
// © Your Studio — All Rights Reserved.

#include "Items/Weapons/WeaponDataAsset.h"
#include "UObject/Package.h"         // GetTransientPackage
#include "UObject/UObjectGlobals.h"  // NewObject
#include "Engine/Texture2D.h"

// If you want logging, uncomment:
// #include "Logging/LogMacros.h"
// DEFINE_LOG_CATEGORY_STATIC(LogWeaponDataAsset, Log, All);

void UWeaponDataAsset::SetWeaponSelected(bool bEquipped)
{
    IsEquipped = bEquipped;

    // Broadcast selection state to any listeners (UI, gameplay systems).
    WeaponEquipped.Broadcast(IsEquipped);
}


void UWeaponDataAsset::BuyWeapon()
{
    // Set purchase state and notify listeners (UI, inventory, analytics, etc.)
    HasBeenBought = true;
    WeaponPurchased.Broadcast(HasBeenBought);
}


float UWeaponDataAsset::GetCurrentDamage() const
{
    // Use a clamped level just in case data is out of bounds.
    const int32 ClampedLevel = FMath::Clamp(CurrentLevel, 0, MaxLevel);

    // Damage model: BaseDamage * DamagePerLevel * Level
    return BaseDamage * DamageScaling * static_cast<float>(ClampedLevel);
}

void UWeaponDataAsset::UpgradeWeapon()
{
    // Increment and clamp: min 0, max MaxLevel
    const int32 OldLevel = CurrentLevel;
    CurrentLevel = FMath::Clamp(CurrentLevel + 1, 0, MaxLevel);
    if (CurrentLevel != OldLevel)
    {
        WeaponUpgraded.Broadcast(CurrentLevel);
    }

}

bool UWeaponDataAsset::CanUpgrade() const
{
    return CurrentLevel < MaxLevel;
}

int32 UWeaponDataAsset::GetPrice() const
{
    // Use a clamped level just in case data is out of bounds.
    const int32 ClampedLevel = FMath::Clamp(CurrentLevel, 0, MaxLevel);

    return Price * PriceScaling * static_cast<float>(ClampedLevel);
}


// =========================
// 🔁 Clone API (existing)
// =========================

UWeaponDataAsset* UWeaponDataAsset::Clone(UObject* Outer) const
{
    // Use caller-provided outer or the transient package to ensure this clone is not saved to disk.
    UObject* UseOuter = Outer ? Outer : GetTransientPackage();

    // Create a transient object of the same class to support future subclassing.
    UWeaponDataAsset* NewClone = NewObject<UWeaponDataAsset>(UseOuter, GetClass(), NAME_None, RF_Transient);
    if (!NewClone)
    {
        return nullptr;
    }

    // Copy all value types and references (do NOT copy multicast delegate bindings).
    NewClone->CopyFrom(this);

    return NewClone;
}

/*static*/ UWeaponDataAsset* UWeaponDataAsset::CloneFrom(const UWeaponDataAsset* Source, UObject* Outer)
{
    return Source ? Source->Clone(Outer) : nullptr;
}

void UWeaponDataAsset::CopyFrom(const UWeaponDataAsset* Source)
{
    if (!Source || Source == this)
    {
        return;
    }

    // ---------- Simple value copies ----------
    WeaponName = Source->WeaponName;
    HasBeenBought = Source->HasBeenBought;
    IsEquipped = Source->IsEquipped;

    Price = Source->Price;
    CurrentLevel = Source->CurrentLevel;
    MaxLevel = Source->MaxLevel;

    BaseDamage = Source->BaseDamage;
    DamageScaling = Source->DamageScaling;
    PriceScaling = Source->PriceScaling;

    // ---------- Class / object references ----------
    WeaponToEquip = Source->WeaponToEquip;

    // ✅ Copy the hard reference to the icon texture (shared asset reference; not duplicated).
    Icon = Source->Icon;

    // NOTE:
    // We intentionally do NOT copy multicast delegate bindings (WeaponSelected, WeaponPurchased).
    // Bind to these on the clone in UI/HUD or gameplay code as needed.
}
