// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrowdExcitementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAZYARENACHAOS_API UCrowdExcitementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCrowdExcitementComponent();

public:	
	void OnHit();
	void OnKill();

private:
	UPROPERTY(EditAnywhere, Category = "Crowd | Excitement")
	float HitExcitementChange{ 0.01f };

	UPROPERTY(EditAnywhere, Category = "Crowd | Excitement")
	float KillExcitementChange{ 0.05f };
};
