// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Currency.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYARENACHAOS_API ACurrency : public AItem
{
	GENERATED_BODY()
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, Category = "Currency Poperties")
	int32 currencyValue;
	
public:
	FORCEINLINE int32 GetCurrency() { return currencyValue; }
};
