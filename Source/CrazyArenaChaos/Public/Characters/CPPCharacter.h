// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Characters/CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "CPPCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

class UGroomComponent;

class UInputMappingContext;
class UInputAction;
class UAttributeComponent;
class AItem; 
class AWeapon; 
class ACurrency;
class UAnimInstance;



UCLASS()
class CRAZYARENACHAOS_API ACPPCharacter : public ACharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ACPPCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	void EquipNewWeapon(TSubclassOf<AWeapon> weaponToEquip);

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starting Weapon")
	TSubclassOf<AWeapon> startingWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	UAttributeComponent* attributeComponent;

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
	void EquipWeapon(AWeapon* overlappingWeapon);
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

	FORCEINLINE ECharacterState GetCharacterState() const { return state; }
	virtual void SetOverlappingItem(AItem* item)override;
	virtual void AddCurrency(ACurrency* currency)override;

	UFUNCTION(BlueprintCallable)
	void EnterShoppingState();
	UFUNCTION(BlueprintCallable)
	void EndShoppingState();

};
