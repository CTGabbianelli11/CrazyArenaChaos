// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "AC_HitStop.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAZYARENACHAOS_API UAC_HitStop : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_HitStop();

protected:
	ACharacter* CharacterActor;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;



	FTimerDynamicDelegate hitStopTimerEvent;
public:	

	UFUNCTION(BlueprintCallable)
	void BeginHitStop(float duration);
	UFUNCTION(BlueprintCallable)
	void EndHitStop();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
