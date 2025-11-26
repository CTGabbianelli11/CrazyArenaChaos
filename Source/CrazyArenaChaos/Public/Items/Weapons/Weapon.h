// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class CRAZYARENACHAOS_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	TArray<AActor*> ignoreActors;
protected:
	 virtual void BeginPlay() override;
	 virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	 virtual void OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	 UFUNCTION()
	 void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBoxComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float damage = 20.f;

	/*
	SFX
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Sound Effect")
	USoundWave* HitSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound Effect")
	USoundWave* MissSoundEffect;
	
public:
	FORCEINLINE UBoxComponent* GetWeaponBoxComponent() const { return WeaponBoxComponent; }
};
