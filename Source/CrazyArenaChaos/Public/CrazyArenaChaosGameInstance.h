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
	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FStructPersistentData playerPersistingAttributes;
};

