// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AC_HitStop.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UAC_HitStop::UAC_HitStop()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	CharacterActor = Cast<ACharacter>(GetOwner());

	hitStopTimerEvent.BindUFunction(this, FName("EndHitStop"));
}


// Called when the game starts
void UAC_HitStop::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAC_HitStop::BeginHitStop(float duration)
{
	if (CharacterActor)
		CharacterActor->CustomTimeDilation = 0;

	UKismetSystemLibrary::K2_SetTimerDelegate(hitStopTimerEvent, duration,false,false);
}

void UAC_HitStop::EndHitStop()
{
	if (CharacterActor)
		CharacterActor->CustomTimeDilation = 0;
}


// Called every frame
void UAC_HitStop::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

