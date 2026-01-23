// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "AC_HitStop.generated.h"

class USkeletalMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAZYARENACHAOS_API UAC_HitStop : public UActorComponent
{
	GENERATED_BODY()

public:	

	
	// Sets default values for this component's properties
	UAC_HitStop();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* skeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitShake")
	FVector MeshRelativeLocation;
protected:
	ACharacter* CharacterActor;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;



	FTimerDynamicDelegate hitStopTimerEvent;
	FTimerDynamicDelegate MeshShakeStepTimerEvent;
	FTimerHandle timerHandle;

	float ShakeAmplitude = 20;
	float offsetDirection = 1;
	float TimeDialation = 1;
	float ShakeSpeed = 1;
public:	

	UFUNCTION(BlueprintCallable)
	void BeginHitStop(float duration, float TimeDialation, float ShakeSpeed, float ShakeAmplitude);
	UFUNCTION(BlueprintCallable)
	void EndHitStop();
	UFUNCTION(BlueprintCallable)
	void ApplyMeshShakeStep();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
