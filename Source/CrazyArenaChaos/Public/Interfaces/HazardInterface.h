// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HazardInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class UHazardInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRAZYARENACHAOS_API IHazardInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual void EnableHazard() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void DisableHazard() = 0;
};
