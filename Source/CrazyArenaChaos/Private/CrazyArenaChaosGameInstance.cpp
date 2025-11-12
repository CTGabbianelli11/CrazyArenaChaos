// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyArenaChaosGameInstance.h"

#include "Components/AttributeComponent.h"

#include "Structs/PersistentDataStruct.h"

void UCrazyArenaChaosGameInstance::ResetAttributes()
{
	playerPersistingAttributes = DefaultPlayerAttributes;
}

void UCrazyArenaChaosGameInstance::Init()
{
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
