
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
class UWeaponDataAsset; // Forward declare for BP helper only

UCLASS()
class CRAZYARENACHAOS_API ACPPCharacter : public ACharacter, public IPickupInterface, public IHitInterface, public IDeathInterface
{
    GENERATED_BODY()

public:
    ACPPCharacter();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Interfaces
    virtual void GetHit(const FVector& impectPoint) override;
    virtual void CharacterDied() override;

    /** Collision gate for the weapon box during attack montage notifies */
    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

    /** Shop-authoritative path: equip an already-spawned weapon instance */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipExistingWeapon(AWeapon* WeaponInstance);

    /**
     * Blueprint helper for Shop UI/logic:
     * Spawns a weapon from a Data Asset, initializes from config once, and optionally equips it.
     * This exists purely to provide a valid World context for Blueprint-only shops.
     */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    AWeapon* BP_SpawnWeaponFromDataAsset(UWeaponDataAsset* Config, bool bEquipNow);

    UAttributeComponent* GetAttributes() { return attributeComponent; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
    UAttributeComponent* attributeComponent;

    /** Currently equipped, live weapon instance */
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

    // Input callbacks
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Interact(const FInputActionValue& Value);

    /** Attach weapon instance without re-initialization (used by interact/store-provided instance). */
    void EquipWeapon(AWeapon* overlappingWeapon);

    void Attack(const FInputActionValue& Value);

    // Play montage functions
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

    UPROPERTY(VisibleAnywhere) UCameraComponent* CameraComponent;
    UPROPERTY(VisibleAnywhere) USpringArmComponent* SpringArm;

    UPROPERTY(VisibleInstanceOnly) AItem* overlappingItem;

    // Animation Montages
    UPROPERTY(EditDefaultsOnly, Category = "Montages") UAnimMontage* AttackMontage;
    UPROPERTY(EditDefaultsOnly, Category = "Montages") UAnimMontage* HitReactMontage;

public:
    FORCEINLINE ECharacterState GetCharacterState() const { return state; }
    virtual void SetOverlappingItem(AItem* item) override;
    virtual void AddCurrency(ACurrency* currency) override;

    UFUNCTION(BlueprintCallable) void EnterShoppingState();
    UFUNCTION(BlueprintCallable) void EndShoppingState();
};
