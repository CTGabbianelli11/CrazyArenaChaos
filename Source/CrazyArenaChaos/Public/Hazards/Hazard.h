// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hazard.generated.h"


UENUM(BlueprintType)
enum class EHazardType : uint8
{
	// Default value, usually 0
	HT_None        UMETA(DisplayName = "None"),
	HT_ExplodingBarrel      UMETA(DisplayName = "Exploding Barrel"),
	HT_FlameThrower      UMETA(DisplayName = "Flame Thrower"),
	HT_Landmine      UMETA(DisplayName = "Landmine"),
	HT_Plane      UMETA(DisplayName = "Plane"),
	HT_RotatingBlade      UMETA(DisplayName = "Rotating Blade")
};





class UCapsuleComponent;

UCLASS()
class CRAZYARENACHAOS_API AHazard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHazard();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard Settings")
	float DamageMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hazard Settings")
	EHazardType HazardType ;
	
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "Hazard Events")
	void ApplyWaveData();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Mesh;
	
	UFUNCTION(BlueprintCallable, Category = "Hazard Events")
	virtual void DeactivateHazard();

};
