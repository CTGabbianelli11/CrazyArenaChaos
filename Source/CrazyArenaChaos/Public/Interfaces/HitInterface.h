// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified (But, I'm gonna do it anyway).
UINTERFACE(BlueprintType,meta = (CannotImplementInterfaceInBlueprint))
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRAZYARENACHAOS_API IHitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual void GetHit(const FVector& impectPoint) = 0;
};
