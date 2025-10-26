// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::RecieveDamage(float _damage)
{
	health = FMath::Clamp(health - _damage, 0,maxHealth);
}

float UAttributeComponent::GetHealthPercentage()
{
	return health/maxHealth;
}

bool UAttributeComponent::IsAlive()
{
	return health > 0;
}

