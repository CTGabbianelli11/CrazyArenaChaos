
// CPPCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Characters/CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/DeathInterface.h"
#include "CPPCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UAttributeComponent;
class AItem;
class AWeapon;
class ACurrency;
class UAnimInstance;
class UWeaponDataAsset;

UCLASS()
class CRAZYARENACHAOS_API ACPPCharacter : public ACharacter, public IPickupInterface, public IHitInterface, public IDeathInterface
{
    GENERATED_BODY()

public:

    ACPPCharacter();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Interface overrides
    virtual void GetHit(const FVector& impectPoint) override;
    virtual void CharacterDied() override;

    // Weapon Collision
    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

    // Equip an existing weapon instance
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipExistingWeapon(AWeapon* WeaponInstance);

    // Spawn from DataAsset
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    AWeapon* BP_SpawnWeaponFromDataAsset(UWeaponDataAsset* Config, bool bEquipNow);

    UAttributeComponent* GetAttributes() { return attributeComponent; }

protected:

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
    UAttributeComponent* attributeComponent;

    UPROPERTY(BlueprintReadWrite, Category = "Weapon")
    AWeapon* equippedWeapon;

    // Input
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

    int attackNumber = 0;

    UFUNCTION(BlueprintImplementableEvent)
    void CharacterDiedEvent();

    // Input Callbacks
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Interact(const FInputActionValue& Value);

    void EquipWeapon(AWeapon* overlappingWeapon);
    void Attack(const FInputActionValue& Value);

    void PlayAttackMontage();

protected:
    UFUNCTION(BlueprintCallable) void AttackEnd();
    UFUNCTION(BlueprintCallable) void StartInputBuffer();
    UFUNCTION(BlueprintCallable) void EndBuffer();
    UFUNCTION(BlueprintCallable) FName GetCurrentAttack();
    bool CanAttack();

private:

    ECharacterState state = ECharacterState::ECS_Unequipped;

    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    EactionState actionState = EactionState::EAS_Unoccupied;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleInstanceOnly)
    AItem* overlappingItem;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* HitReactMontage;

public:

    FORCEINLINE ECharacterState GetCharacterState() const { return state; }

    // ----------- FIXED INTERFACE OVERRIDES -----------
    virtual void SetOverlappingItem_Implementation(AItem* Item) override;
    virtual void AddCurrency_Implementation(ACurrency* Currency) override;

    UFUNCTION(BlueprintCallable)
    void EnterShoppingState();

    UFUNCTION(BlueprintCallable)
    void EndShoppingState();
};
