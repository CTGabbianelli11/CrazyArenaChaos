// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_Cone.h"
#include "FFC_EnvQG_GetBackCone.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYARENACHAOS_API UFFC_EnvQG_GetBackCone : public UEnvQueryGenerator_Cone
{
	GENERATED_BODY()
	

public:
	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
};
