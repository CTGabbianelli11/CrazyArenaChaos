// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Characters/CPPCharacter.h"
#include "Components/AttributeComponent.h"
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
	UWorld* world = GetWorld();
	if (world)
	{
		ACPPCharacter* character = Cast<ACPPCharacter>(UGameplayStatics::GetPlayerCharacter(world, 0));
		if (character)
		{
			if (character->GetAttributes()->RemoveCurrency(weaponDataAsset->Price))
				ItemBoughtChanged(true);
		}
		weaponDataAsset->BuyWeapon();
	}
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
