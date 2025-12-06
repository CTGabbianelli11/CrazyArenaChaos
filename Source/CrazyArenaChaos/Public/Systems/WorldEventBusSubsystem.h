// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldEvents.h"
#include "WorldEventBusSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYARENACHAOS_API UWorldEventBusSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Notification | Crowd", Transient, BlueprintAssignable, BlueprintCallable)
	FOnCrowdExcitementUpdated OnCrowdExcitementUpdated;
	
};
