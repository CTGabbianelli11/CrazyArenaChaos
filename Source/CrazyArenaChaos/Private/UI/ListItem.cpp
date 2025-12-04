// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ListItem.h"

void UListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (ListItemObject&&Cast<UWeaponDataAsset>(ListItemObject))
	{
		weaponDataAsset = Cast<UWeaponDataAsset>(ListItemObject);

		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, weaponDataAsset->WeaponName);
	}
}
