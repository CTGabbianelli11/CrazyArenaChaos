// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ListItem.h"

void UListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (ListItemObject&&Cast<UWeaponDataAsset>(ListItemObject))
	{
		weaponDataAsset = Cast<UWeaponDataAsset>(ListItemObject);

		WeaponText->SetText(FText::FromString(weaponDataAsset->WeaponName));



		ItemBoughtChanged(weaponDataAsset->HasBeenBought);

		ItemEquipChanged(weaponDataAsset->IsEquipped);


		BuyButton->OnClicked.AddDynamic(this, &UListItem::TryToBuyItem);

	}
}

void UListItem::TryToBuyItem()
{
	weaponDataAsset->BuyWeapon();
}

void UListItem::ItemBoughtChanged(bool itemBought)
{
	if (itemBought)
	{
		BuyButton->SetVisibility(ESlateVisibility::Hidden);
		
		EquipButton->SetVisibility(ESlateVisibility::Visible);

		UpgradeButton->SetVisibility(ESlateVisibility::Visible);

	}
	else
	{
		BuyButton->SetVisibility(ESlateVisibility::Visible);

		EquipButton->SetVisibility(ESlateVisibility::Hidden);

		UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UListItem::ItemEquipChanged(bool itemEquipped)
{
	if(itemEquipped)
	{
		EquipButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		EquipButton->SetVisibility(ESlateVisibility::Visible);
	}

}
