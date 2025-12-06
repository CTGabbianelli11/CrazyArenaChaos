// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/ListItem.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CPPCharacter.h"
#include "Components/AttributeComponent.h"
#include "CrazyArenaChaosGameInstance.h"



void UListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UWorld* world = GetWorld();
	if (world)
	{
		character = Cast<ACPPCharacter>(UGameplayStatics::GetPlayerCharacter(world, 0));
	}

	gameInstance = Cast<UCrazyArenaChaosGameInstance>(GetGameInstance());

	if (ListItemObject&&Cast<UWeaponDataAsset>(ListItemObject))
	{
		weaponDataAsset = Cast<UWeaponDataAsset>(ListItemObject);

		WeaponText->SetText(FText::FromString(weaponDataAsset->WeaponName));



		ItemBoughtChanged(weaponDataAsset->HasBeenBought);

		ItemEquipChanged(weaponDataAsset->IsEquipped);


		BuyButton->OnClicked.AddDynamic(this, &UListItem::TryToBuyItem);
		EquipButton->OnClicked.AddDynamic(this, &UListItem::EquipItem);

	}
}

void UListItem::TryToBuyItem()
{
		if (character)
		{
			if (character->GetAttributes()->RemoveCurrency(weaponDataAsset->Price))
				ItemBoughtChanged(true);
		}
		weaponDataAsset->BuyWeapon();
}

void UListItem::EquipItem()
{
	if (weaponDataAsset->HasBeenBought)
	{
		gameInstance->SelectWeapon(weaponDataAsset);
		ItemEquipChanged(true);
	}

}

void UListItem::ItemBoughtChanged(bool itemBought)
{
	if (itemBought)
	{
		weaponDataAsset->HasBeenBought = true;

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
		ItemEquippedEvent.Broadcast();
	}
	else
	{
		EquipButton->SetVisibility(ESlateVisibility::Visible);
	}

}
