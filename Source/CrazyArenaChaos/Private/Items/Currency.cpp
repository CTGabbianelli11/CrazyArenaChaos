// Fill out your copyright notice in the Description page of Project Settings.
#include "Items/Currency.h"
#include "Characters/CPPCharacter.h"


void ACurrency::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		ACPPCharacter* character = Cast<ACPPCharacter>(OtherActor);
		if (character)
		{
			character->AddCurrency(currencyValue);
			Destroy();
		}
	}
}
