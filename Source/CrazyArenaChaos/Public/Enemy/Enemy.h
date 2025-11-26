// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/DeathInterface.h"	
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UNiagaraSystem;

UCLASS()
class CRAZYARENACHAOS_API AEnemy : public ACharacter,public IHitInterface,public IDeathInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	UPROPERTY(EditAnywhere)
	UClass* CurrencyToDrop;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const FVector& impectPoint) override;
	virtual void CharacterDied() override;

	void DirectionalHitReact(const FVector& impactPoint,const FVector impactDirection);
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	UFUNCTION(BlueprintCallable)
	void EnableRagdoll();
	UFUNCTION(BlueprintCallable)

	void DropCurrency();
	/**
Play montage functions
*/
	void PlayHitReactMontage(const FName& sectionName);
protected:
	virtual void BeginPlay() override;
private:
	/*
	*Animation Montages
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	/*
	VFX
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Particle System")
	UNiagaraSystem* HitSystem;



	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* attributes;

	UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
	USoundWave* hitSound;
public:	

};
