// Fill out your copyright notice in the Description page of Project Settings.
#include "Items/Currency.h"
#include "Interfaces/PickupInterface.h"
#include "Characters/CPPCharacter.h"


void ACurrency::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		IPickupInterface* HitPickup = Cast<IPickupInterface>(OtherActor);
		if (HitPickup)
		{
			HitPickup->AddCurrency(this);
			Destroy();
		}
	}
}
