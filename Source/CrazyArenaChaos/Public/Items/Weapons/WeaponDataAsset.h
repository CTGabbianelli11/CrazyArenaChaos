// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"
/**
 *
 */
class AWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponEquipped, bool, IsEquipped);
/** New: purchase event */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponPurchased, bool, HasBeenBought);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponUpgraded, int32, NewLevel);


UCLASS(Blueprintable, BlueprintType)
class CRAZYARENACHAOS_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()


public:

    // WeaponDataAsset.h (add inside public: or a section you prefer)
    UPROPERTY(BlueprintReadWrite, Category = "Weapon|Runtime", meta = (DisplayName = "Equipped Weapon Instance"))
    AWeapon* WeaponInstance = nullptr;

    /** ---------- Identity / Type ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FString WeaponName = TEXT("Generic Weapon");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> WeaponToEquip;

    /** ---------- Ownership / State ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|State")
    bool HasBeenBought = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|State")
    bool IsEquipped = false;

    /** ---------- Economy ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Economy", meta = (ClampMin = "0"))
    int32 Price = 0;

    /** ---------- Leveling ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Level", meta = (ClampMin = "0"))
    int32 CurrentLevel = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Level", meta = (ClampMin = "0"))
    int32 MaxLevel = 5;

    /** ---------- Damage ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage", meta = (ClampMin = "0.0"))
    float BaseDamage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage", meta = (ClampMin = "0.0"))
    float DamageScaling = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Economy", meta = (ClampMin = "0.0"))
    float PriceScaling = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
    float SizeScaleFactor = 0.0f;

    /** ---------- UI ---------- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|UI")
    UTexture2D* Icon = nullptr;


public:

    /** Selection API (existing) */
    UFUNCTION(BlueprintCallable, Category = "Weapon|State")
    void SetWeaponSelected(bool bEquipped);

    /** Purchase API (updated to broadcast) */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Shop")
    void BuyWeapon();

    /** Leveling API */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
    void UpgradeWeapon();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Level")
    bool CanUpgrade() const;

    /** Damage API */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Damage")
    float GetCurrentDamage() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Economy")
    int32 GetPrice() const;

public:

    /** Events */
    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FWeaponEquipped WeaponEquipped;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FWeaponPurchased WeaponPurchased;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FWeaponUpgraded WeaponUpgraded;

    /** =========================
	 *  🔁 Clone API (additions)
	 *  ========================= */

	 /** Create a runtime clone (transient) of this data asset.
	  *  @param Outer Owner of the clone (use e.g. a widget, TileView, or GetTransientPackage()).
	  *  @return New transient UWeaponDataAsset with copied fields.
	  */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Clone")
	UWeaponDataAsset* Clone(UObject* Outer) const;

	/** Convenience: clone from a source asset.
	 *  @param Source Source asset to copy from.
	 *  @param Outer  Owner for the clone.
	 *  @return New transient clone or nullptr if Source is null.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Clone")
	static UWeaponDataAsset* CloneFrom(const UWeaponDataAsset* Source, UObject* Outer);

protected:
	/** Internal copier used by Clone/CloneFrom. Does not copy delegate bindings. */
	void CopyFrom(const UWeaponDataAsset* Source);


};
