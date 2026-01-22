// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PersistentDataStruct.generated.h"


USTRUCT(BlueprintType)
struct FWaveInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 WaveNumber{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float EnemyHealthMultiplier{1.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float EnemyDamageMultiplier{1.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 MinEnemies{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 MaxEnemies{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float HazardDamage{1.0f};
};


USTRUCT(BlueprintType)
struct FStructPersistentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persistent Data")
	float currency{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persistent Data")
	int32 WaveNumber{};
	
};


