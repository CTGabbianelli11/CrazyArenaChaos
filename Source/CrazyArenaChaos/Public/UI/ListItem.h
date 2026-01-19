
// ListItem.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ListItem.generated.h"

class ACPPCharacter;
class UCrazyArenaChaosGameInstance;
class AWeapon;
class UTexture2D;

/** Broadcast when this entry equips (for a parent list to clear other entries' equip state) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemEquipped);

UCLASS()
class CRAZYARENACHAOS_API UListItem : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

public:
    /** Bound widgets (names must match UMG) */
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UButton* EquipButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UButton* BuyButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UButton* UpgradeButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* WeaponText;

    /** The runtime weapon instance this row represents (created by the Shop) */
    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    AWeapon* WeaponInstance = nullptr;

public:
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

    /** UI actions */
    UFUNCTION(BlueprintCallable)
    void TryToBuyItem();

    UFUNCTION(BlueprintCallable)
    void EquipItem();

    /** Update visuals for purchased state (UI-only) */
    UFUNCTION(BlueprintCallable)
    void ItemBoughtChanged(bool bItemBought);

    /** Update visuals for equipped state */
    UFUNCTION(BlueprintCallable)
    void ItemEquipChanged(bool bItemEquipped);

public:
    /** Fired when this row equips so external UI can update other rows */
    UPROPERTY(BlueprintAssignable)
    FItemEquipped ItemEquippedEvent;

private:
    ACPPCharacter* character = nullptr;
    UCrazyArenaChaosGameInstance* gameInstance = nullptr;

    /** Local UI flags (authoritative ownership can come from Shop) */
    bool bBought = false;
    bool bEquipped = false;
};
