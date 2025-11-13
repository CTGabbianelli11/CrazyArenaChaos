// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"
/**
 * 
 */
class AWeapon;
UCLASS(Blueprintable, BlueprintType)
class CRAZYARENACHAOS_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString WeaponName="Generic Weapon";
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AWeapon> WeaponToEquip;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool HasBeenBought=false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEquipped=false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;
	
};
