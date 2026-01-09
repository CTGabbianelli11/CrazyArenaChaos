// Fill out your copyright notice in the Description page of Project Settings.


#include "FFC/FFC_EnvQG_GetBackCone.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

void UFFC_EnvQG_GetBackCone::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	//Super::GenerateItems(QueryInstance);
	TArray<AActor*> CenterActors;
	QueryInstance.PrepareContext(CenterActor, CenterActors);

	if (CenterActors.Num() <= 0)
	{
		return;
	}

	BindDataToDataProviders(QueryInstance);

	const float ConeDegreesValue = FMath::Clamp(FMath::Abs(ConeDegrees.GetValue()), 0.f, 359.f);
	if (ConeDegreesValue == 0)
	{
		return;
	}

	//Get the values from each data provider
	const float AlignedPointDistanceValue = AlignedPointsDistance.GetValue();
	const float AngleStepValue = FMath::Clamp(AngleStep.GetValue(), 1.f, 359.f);
	const float GenerationRange = FMath::Clamp(Range.GetValue(), 0.f, MAX_flt);
	const int32 MaxPointsPerAngleValue = FMath::FloorToInt(GenerationRange / AlignedPointDistanceValue);

	TArray<FNavLocation> GeneratedItems;
	GeneratedItems.Reserve(CenterActors.Num() * FMath::CeilToInt(ConeDegreesValue / AngleStepValue) * MaxPointsPerAngleValue + 1);

	//Generate points for each actor
	for (int32 CenterIndex = 0; CenterIndex < CenterActors.Num(); CenterIndex++)
	{
		const FVector ForwardVector = CenterActors[CenterIndex]->GetActorForwardVector();
		const FVector ActorLocation = CenterActors[CenterIndex]->GetActorLocation();

		for (float Angle = -(ConeDegreesValue * 0.5f); Angle < (ConeDegreesValue * 0.5f); Angle += AngleStepValue)
		{
			//invert forward vec to get cone behind
			const FVector AxisDirection = -1.f*ForwardVector.RotateAngleAxis(Angle, FVector::UpVector);

			// skipping PointIndex == 0 as that's the context's location
			for (int32 PointIndex = 1; PointIndex <= MaxPointsPerAngleValue; PointIndex++)
			{
				const FVector GeneratedLocation = ActorLocation + (AxisDirection * PointIndex * AlignedPointDistanceValue);
				GeneratedItems.Add(FNavLocation(GeneratedLocation));
			}
		}

		if (bIncludeContextLocation)
		{
			GeneratedItems.Add(FNavLocation(ActorLocation));
		}
	}

	ProjectAndFilterNavPoints(GeneratedItems, QueryInstance);
	StoreNavPoints(GeneratedItems, QueryInstance);

}

FText UFFC_EnvQG_GetBackCone::GetDescriptionTitle() const
{
	return FText::Format(LOCTEXT("ConeDescriptionGenerateAroundContext", "Cone : generate behind {0}"),
		UEnvQueryTypes::DescribeContext(CenterActor));

}
