// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CrowdConfiguration.generated.h"


UCLASS(BlueprintType)
class CRAZYARENACHAOS_API UCrowdConfiguration : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> CrowdDataByWave{};
};
