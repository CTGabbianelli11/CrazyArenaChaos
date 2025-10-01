// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	void Equip(USceneComponent* InParent, FName InSocketName);

	UFUNCTION(BlueprintCallable)
	void StartColliders();
	UFUNCTION(BlueprintCallable)
	void EndColliders();

	UFUNCTION(BlueprintImplementableEvent)
	void CollidersStartedEvent();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CollidersStoppedEvent();
protected:

	 void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	 void OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
