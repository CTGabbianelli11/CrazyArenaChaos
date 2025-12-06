// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CrowdExcitementComponent.h"

#include "Systems/WorldEvents.h"
#include "Systems/WorldEventBusSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CrowdExcitementComponent)

UCrowdExcitementComponent::UCrowdExcitementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCrowdExcitementComponent::OnHit()
{
	if(auto WorldEvents = GetWorld()->GetSubsystem<UWorldEventBusSubsystem>(); WorldEvents)
	{
		WorldEvents->OnCrowdExcitementUpdated.Broadcast(HitExcitementChange);
	}
}

void UCrowdExcitementComponent::OnKill()
{
	if (auto WorldEvents = GetWorld()->GetSubsystem<UWorldEventBusSubsystem>(); WorldEvents)
	{
		WorldEvents->OnCrowdExcitementUpdated.Broadcast(KillExcitementChange);
	}
}
