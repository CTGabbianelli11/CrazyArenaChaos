// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"
/**
 *
 */
class AWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponSelected, bool, IsEquipped);

UCLASS(Blueprintable, BlueprintType)
class CRAZYARENACHAOS_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponName = "Generic Weapon";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> WeaponToEquip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBeenBought = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;

	// ✅ New properties for leveling
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel = 5;

	/** ✅ Shop/Icon image (hard reference) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|UI")
	UTexture2D* Icon = nullptr;


public:
	UFUNCTION(BlueprintCallable)
	void SetWeaponSelected(bool equipped);
	UFUNCTION(BlueprintCallable)
	void BuyWeapon();

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWeaponSelected WeaponSelected;

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
