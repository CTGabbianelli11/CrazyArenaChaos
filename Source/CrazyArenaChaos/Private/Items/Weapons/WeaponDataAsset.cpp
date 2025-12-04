// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WeaponDataAsset.h"
#include "Characters/CPPCharacter.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"

void UWeaponDataAsset::SetWeaponSelected(bool equipped)
{
	IsEquipped = equipped;
	WeaponSelected.Broadcast(IsEquipped);
}

void UWeaponDataAsset::BuyWeapon()
{

}
