// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs/PersistentDataStruct.h"
#include "CrazyArenaChaosGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYARENACHAOS_API UCrazyArenaChaosGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	/**
	* Resets the persistent data to initial values.
	* Possibly could be used from main menu when a new game is started, assuming currency is not persisted across sessions.
	*/
	UFUNCTION(BlueprintCallable, Category = "Persistent Data")
	void ResetAttributes();

	virtual void Init() override;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FStructPersistentData playerPersistingAttributes{};

private:
	FStructPersistentData DefaultPlayerAttributes{};

};
