// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs/PersistentDataStruct.h"
#include "CrazyArenaChaosGameInstance.generated.h"

class AEnemy;
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
	AActor* LevelTransitionDoor;
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
	void AddEnemy(AEnemy* Enemy);
	UFUNCTION(BlueprintCallable)
	void RemoveEnemy(AEnemy* Enemy);
#pragma endregion

#pragma region Events
public:
	UPROPERTY(BlueprintAssignable)
	FAllEnemiesDefeated AllEnemiesDefeated;
#pragma endregion



};
