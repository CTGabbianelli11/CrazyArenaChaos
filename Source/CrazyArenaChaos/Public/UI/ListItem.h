// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Items/Weapons/WeaponDataAsset.h"
#include "Components/Button.h"
#include "ListItem.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYARENACHAOS_API UListItem : public UUserWidget,public IUserObjectListEntry
{

	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	UButton* EquipButton; // Name this exactly as your button in UMG
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BuyButton; // Name this exactly as your button in UMG
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* UpgradeButton; // Name this exactly as your button in UMG

	UPROPERTY(BlueprintReadWrite)
	UWeaponDataAsset* weaponDataAsset;

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
