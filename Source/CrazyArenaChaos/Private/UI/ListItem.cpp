
// ListItem.cpp
#include "UI/ListItem.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CPPCharacter.h"
#include "Components/AttributeComponent.h"
#include "CrazyArenaChaosGameInstance.h"
#include "Items/Weapons/Weapon.h" // for getters like GetIcon/GetPrice

void UListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

    UWorld* World = GetWorld();
    if (World)
    {
        character = Cast<ACPPCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
    }
    gameInstance = Cast<UCrazyArenaChaosGameInstance>(GetGameInstance());

    // Expect the ListView to pass an AWeapon instance (created/provisioned by the Shop)
    WeaponInstance = Cast<AWeapon>(ListItemObject);

    if (WeaponInstance && WeaponText)
    {
        // If your AWeapon exposes a display name, use it; otherwise show class name
        const FString Label = WeaponInstance->GetName(); // replace with your own display if available
        WeaponText->SetText(FText::FromString(Label));
    }

    // Initialize UI-only state from the weapon’s runtime flags if you want
    bEquipped = (WeaponInstance && WeaponInstance->IsEquipped());
    bBought = (WeaponInstance && WeaponInstance->IsPurchased());

    ItemBoughtChanged(bBought);
    ItemEquipChanged(bEquipped);

    if (BuyButton)
    {
        BuyButton->OnClicked.RemoveAll(this);
        BuyButton->OnClicked.AddDynamic(this, &UListItem::TryToBuyItem);
    }
    if (EquipButton)
    {
        EquipButton->OnClicked.RemoveAll(this);
        EquipButton->OnClicked.AddDynamic(this, &UListItem::EquipItem);
    }
}

void UListItem::TryToBuyItem()
{
    if (!character || !WeaponInstance) return;

    // Deduct base price locally (UI sample). You can route this through a server Shop later.
    const int32 PriceToPay = WeaponInstance->GetPrice();
    if (UAttributeComponent* Attr = character->GetAttributes())
    {
        if (Attr->RemoveCurrency(PriceToPay))
        {
            // Mark purchase on the weapon instance and update UI
            WeaponInstance->BuyWeapon();
            ItemBoughtChanged(true);
        }
    }
}

void UListItem::EquipItem()
{
    if (!WeaponInstance || !character) return;

    // Only allow equip if purchased
    if (bBought || WeaponInstance->IsPurchased())
    {
        // Equip this specific instance on the character
        character->EquipExistingWeapon(WeaponInstance);

        // Update UI state locally
        ItemEquipChanged(true);

        // Inform parent list to clear equip state on other entries
        ItemEquippedEvent.Broadcast();
    }
}

void UListItem::ItemBoughtChanged(bool bItemBought)
{
    bBought = bItemBought;

    if (BuyButton)
        BuyButton->SetVisibility(bBought ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

    if (EquipButton)
        EquipButton->SetVisibility(bBought ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    if (UpgradeButton)
        UpgradeButton->SetVisibility(bBought ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UListItem::ItemEquipChanged(bool bItemEquipped)
{
    bEquipped = bItemEquipped;

    if (EquipButton)
    {
        EquipButton->SetVisibility(bEquipped ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
    // In your master list, bind ItemEquippedEvent from each row and call ItemEquipChanged(false) on all others.
}
