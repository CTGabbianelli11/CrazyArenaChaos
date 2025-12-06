// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyArenaChaosGameInstance.h"

#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CPPCharacter.h"
#include "Items/Weapons/WeaponDataAsset.h"
#include "Structs/PersistentDataStruct.h"

void UCrazyArenaChaosGameInstance::ResetAttributes()
{
	playerPersistingAttributes = DefaultPlayerAttributes;
}

void UCrazyArenaChaosGameInstance::SelectWeapon(UWeaponDataAsset* WeaponDataAsset)
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (ACPPCharacter* character = Cast<ACPPCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0)))
		{
			if (CurrentWeapon)
				CurrentWeapon->SetWeaponSelected(false);

			CurrentWeapon = WeaponDataAsset;

			CurrentWeapon->SetWeaponSelected(true);
			character->EquipNewWeapon(CurrentWeapon->WeaponToEquip);
		}
	}

}

void UCrazyArenaChaosGameInstance::Init()
{
	if (UWorld* World = GetWorld())
	{

	}

	DefaultPlayerAttributes = playerPersistingAttributes;
}

void UCrazyArenaChaosGameInstance::AddEnemy(AEnemy* Enemy)
{
	EnemiesToBeDefeated.Add(Enemy);
}

void UCrazyArenaChaosGameInstance::RemoveEnemy(AEnemy* Enemy)
{
	EnemiesToBeDefeated.Remove(Enemy);

	if (EnemiesToBeDefeated.Num() <= 0)
	{
		AllEnemiesDefeated.Broadcast();
		AllEnemiesDefeatedEvent();
		//GetWorld()->SpawnActor(LevelTransiti);
	}
}

void UCrazyArenaChaosGameInstance::LoadShop()
{
	UWorld* World = GetWorld();
	
	if (World)
		UGameplayStatics::OpenLevelBySoftObjectPtr(World, ShopLevel);
}
