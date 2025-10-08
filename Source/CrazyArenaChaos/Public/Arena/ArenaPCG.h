// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "ArenaPCG.generated.h"

class UPCGComponent;
class USplineComponent;

UCLASS()
class CRAZYARENACHAOS_API AArenaPCG : public AActor
{
	GENERATED_BODY()

public:	
	AArenaPCG();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Components", VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Components", VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(BlueprintReadOnly, Category = "Components", VisibleAnywhere)
	TObjectPtr<UPCGComponent> PCG;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Radius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 NumPoints;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bGenerate;
};