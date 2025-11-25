// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs/PersistentDataStruct.h"
#include "CrazyArenaChaosGameInstance.generated.h"

class AEnemy;
class UWeaponDataAsset;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllEnemiesDefeated);

UCLASS()
class CRAZYARENACHAOS_API UCrazyArenaChaosGameInstance : public UGameInstance
{
	GENERATED_BODY()

#pragma region Variables
public:
	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FStructPersistentData playerPersistingAttributes{};

	UPROPERTY(BlueprintReadOnly, Category = "Wave Attributes")
	TArray<AEnemy*> EnemiesToBeDefeated;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> LevelTransitionDoor;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon Attributes")
	TArray<UWeaponDataAsset*> Weapons;

	UPROPERTY(BlueprintReadOnly,Category = "Level Transition Attributes")
	TSoftObjectPtr<UWorld> ShopLevel;
private:
	FStructPersistentData DefaultPlayerAttributes{};


#pragma endregion

#pragma region Functions
public:
	virtual void Init() override;
	/**
* Resets the persistent data to initial values.
* Possibly could be used from main menu when a new game is started, assuming currency is not persisted across sessions.
*/
	UFUNCTION(BlueprintCallable, Category = "Persistent Data")
	void ResetAttributes();
	UFUNCTION(BlueprintCallable)
	void SelectWeapon(UWeaponDataAsset* WeaponDataAsset);
	UFUNCTION(BlueprintCallable)
	void AddEnemy(AEnemy* Enemy);
	UFUNCTION(BlueprintCallable)
	void RemoveEnemy(AEnemy* Enemy);
	/*
	Handle Level transitions
	*/
	UFUNCTION(BlueprintCallable)
	void LoadShop();
#pragma endregion

#pragma region Events
public:
	UPROPERTY(BlueprintAssignable)
	FAllEnemiesDefeated AllEnemiesDefeated;

	UFUNCTION(BlueprintImplementableEvent)
	void AllEnemiesDefeatedEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerDiedEvent();
#pragma endregion



};
