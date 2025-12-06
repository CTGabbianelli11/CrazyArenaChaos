// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WorldEvents.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCrowdExcitementUpdated, float, ExcitementDelta);

// This is required in order for the above delegate declaration to code expand - otherwise the macros do nothing
UCLASS()
class UWorldEventsDummyClass : public UObject
{
	GENERATED_BODY()
};
