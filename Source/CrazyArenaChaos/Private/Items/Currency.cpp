// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CPPCharacter.h"
#include "Items/Currency.h"

void ACurrency::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		ACPPCharacter* character = Cast<ACPPCharacter>(OtherActor);
		if (character)
		{
			Destroy();
		}
	}
}
