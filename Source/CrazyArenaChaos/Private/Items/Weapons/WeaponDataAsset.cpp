// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WeaponDataAsset.h"

void UWeaponDataAsset::SetWeaponSelected(bool equipped)
{
	IsEquipped = equipped;
	WeaponSelected.Broadcast(IsEquipped);
}
