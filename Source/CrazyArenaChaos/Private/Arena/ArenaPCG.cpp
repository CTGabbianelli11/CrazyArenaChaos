// Fill out your copyright notice in the Description page of Project Settings.

#include "Arena/ArenaPCG.h"

#include "Components/SplineComponent.h"
#include "PCGComponent.h"

AArenaPCG::AArenaPCG()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(DefaultSceneComponent);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(GetRootComponent());

	PCG = CreateDefaultSubobject<UPCGComponent>(TEXT("PCG"));
	PCG->bIsComponentPartitioned = false;

	Radius = 100.0f;
	NumPoints = 4;
	bGenerate = true;
}

void AArenaPCG::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bGenerate)
	{
		Spline->ClearSplinePoints();

		ensureMsgf(NumPoints > 0, TEXT("NumPoints must be > 0"));

		for (int32 i = 0; i < NumPoints; i++)
		{
			float Angle = -360.0f * (1.0f / NumPoints) * i;

			FVector Forward = GetActorForwardVector();
			FVector Right = GetActorRightVector();
			FVector Up = GetActorUpVector();

			double Distance = 4.0 * FMath::Tan(PI / (2.0 * NumPoints)) * Radius;

			FVector Location = Right.RotateAngleAxis(Angle, Up) * Radius;
			FVector Tangent = Forward.RotateAngleAxis(Angle, Up) * Distance;

			FSplinePoint Point(i, Location, ESplinePointType::CurveCustomTangent);
			Point.Rotation = FRotator(0.0, Angle, 0.0);
			Point.ArriveTangent = Tangent;
			Point.LeaveTangent = Tangent;

			Spline->AddPoint(Point);
		}

		Spline->SetClosedLoopAtPosition(true, NumPoints, true);
	}
}

void AArenaPCG::BeginPlay()
{
	Super::BeginPlay();
}

void AArenaPCG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}