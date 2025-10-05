// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Characters/CharacterTypes.h"
#include "CPPCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

class UGroomComponent;

class UInputMappingContext;
class UInputAction;
class AItem; 
class AWeapon; 
class UAnimInstance;



UCLASS()
class CRAZYARENACHAOS_API ACPPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACPPCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	AWeapon* equippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* CharacterInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;

	/*
	Call backs for inputs
	*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	/**
	Play montage functions
	*/
	void PlayAttackMontage();
protected:

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();
private:
	ECharacterState state = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EactionState actionState = EactionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	//UPROPERTY(VisibleAnywhere, Category = Hair)
	//UGroomComponent* hair;

	//UPROPERTY(VisibleAnywhere, Category = Hair)
	//UGroomComponent* eyeBrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* overlappingItem;
	




/*
*Animation Montages
*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;
public:
	FORCEINLINE void SetOverlappingItem(AItem* item) { overlappingItem = item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return state; }
};
