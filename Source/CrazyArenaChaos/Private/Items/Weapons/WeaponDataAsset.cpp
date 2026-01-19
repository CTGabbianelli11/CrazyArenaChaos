
#include "Items/Weapons/WeaponDataAsset.h"
#include "Items/Weapons/Weapon.h"

void UWeaponDataAsset::PostLoad()
{
    Super::PostLoad();
    EnsureStableIdentity();
}

void UWeaponDataAsset::EnsureStableIdentity()
{
    if (StableName.IsNone())
    {
        StableName = GetFName();
    }
    if (!StableGuid.IsValid())
    {
        StableGuid = FGuid::NewGuid();
    }
}

FGuid UWeaponDataAsset::GetStableGUID() const {
    return StableGuid;
}
