// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PersistentDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FStructPersistentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Persistent Data")
	float currency;
};