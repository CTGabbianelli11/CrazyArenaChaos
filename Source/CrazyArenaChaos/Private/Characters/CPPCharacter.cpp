
// CPPCharacter.cpp
#include "Characters/CPPCharacter.h"
#include "Animation/AnimMontage.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <EnhancedInputComponent.h>
#include "Items/Item.h"
#include "Components/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Currency.h"
#include "Components/BoxComponent.h"
#include "CrazyArenaChaosGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Included only for the BP helper that spawns from Data Asset
#include "Items/Weapons/WeaponDataAsset.h"

ACPPCharacter::ACPPCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->TargetArmLength = 300.f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArm);

    attributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ACPPCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(CharacterInputMappingContext, 0);
        }
    }

    // Load persisted currency
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UCrazyArenaChaosGameInstance* CAC_GI = Cast<UCrazyArenaChaosGameInstance>(GI))
        {
            attributeComponent->AddCurrency(CAC_GI->playerPersistingAttributes.currency);
        }
    }
}

void ACPPCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D movementVector = Value.Get<FVector2D>();
    const FRotator controlRotation = GetControlRotation();
    const FRotator YawRotation(0.f, controlRotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, movementVector.Y);

    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, movementVector.X);
}

void ACPPCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D lookValue = Value.Get<FVector2D>();
    AddControllerYawInput(lookValue.X);
    AddControllerPitchInput(-lookValue.Y);
}

void ACPPCharacter::Interact(const FInputActionValue& /*Value*/)
{
    // Equip overlapping world weapon instance (e.g., dropped item that is already an AWeapon)
    if (AWeapon* overlappingWeapon = Cast<AWeapon>(overlappingItem))
    {
        EquipExistingWeapon(overlappingWeapon);
    }
}

void ACPPCharacter::EquipWeapon(AWeapon* overlappingWeapon)
{
    if (!overlappingWeapon) return;

    // Remove previously equipped weapon
    if (equippedWeapon != nullptr)
    {
        // Hide it safely if you don’t render inventory in-world
        equippedWeapon->Unequip(/*bReturnToInventory=*/false);
        //equippedWeapon->Destroy();
    }

    // Attach; AWeapon::Equip will re-apply runtime scale AFTER attachment (fixes scale mismatch)
    overlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
    equippedWeapon = overlappingWeapon;
    state = ECharacterState::ECS_EquippedOneHandedWeapon;

    // Mark as equipped (fires weapon-side event); do NOT re-initialize here
    equippedWeapon->SetEquipped(true);
}

void ACPPCharacter::EquipExistingWeapon(AWeapon* WeaponInstance)
{
    EquipWeapon(WeaponInstance);
}

AWeapon* ACPPCharacter::BP_SpawnWeaponFromDataAsset(UWeaponDataAsset* Config, bool bEquipNow)
{
    if (!Config || !Config->WeaponToEquip) return nullptr;

    UWorld* World = GetWorld();
    if (!World) return nullptr;

    // Spawn runtime weapon from the class indicated in the Data Asset
    AWeapon* NewWeapon = World->SpawnActor<AWeapon>(Config->WeaponToEquip);
    if (!NewWeapon) return nullptr;

    // One-time, immutable -> runtime copy
    NewWeapon->InitializeFromDataAsset(Config);

    if (bEquipNow)
    {
        EquipExistingWeapon(NewWeapon);
    }

    return NewWeapon;
}

void ACPPCharacter::Attack(const FInputActionValue& /*Value*/)
{
    if (CanAttack())
    {
        PlayAttackMontage();
        actionState = EactionState::EAS_Attacking;
    }
}

void ACPPCharacter::EnterShoppingState()
{
    if (UWorld* World = GetWorld())
    {
        FRotator RotateTo = UGameplayStatics::GetPlayerCharacter(World, 0)->GetActorRotation();
        RotateTo.Yaw += 180;
        GetController()->SetControlRotation(RotateTo);
    }
}

void ACPPCharacter::EndShoppingState()
{
    if (UWorld* World = GetWorld())
    {
        GetController()->SetControlRotation(FRotator::ZeroRotator);
    }
}

bool ACPPCharacter::CanAttack()
{
    return (actionState == EactionState::EAS_Unoccupied || actionState == EactionState::EAS_Comboing) &&
        state != ECharacterState::ECS_Unequipped;
}

void ACPPCharacter::GetHit(const FVector& /*impactPoint*/)
{
    // Intentionally left blank
}

void ACPPCharacter::CharacterDied()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, TEXT("Dead"));
    GetMesh()->SetAnimInstanceClass(nullptr);

    if (UWorld* World = GetWorld())
    {
        ACharacter* character = UGameplayStatics::GetPlayerCharacter(this, 0);
        GetMesh()->SetSimulatePhysics(true);
        GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
    }

    CharacterDiedEvent();
}

void ACPPCharacter::PlayAttackMontage()
{
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        if (AttackMontage)
        {
            AnimInstance->Montage_Play(AttackMontage);
            AnimInstance->Montage_JumpToSection(GetCurrentAttack(), AttackMontage);
        }
    }
}

void ACPPCharacter::AttackEnd()
{
    actionState = EactionState::EAS_Unoccupied;
    attackNumber = 0;
}

void ACPPCharacter::StartInputBuffer()
{
    actionState = EactionState::EAS_Comboing;
}

void ACPPCharacter::EndBuffer()
{
    actionState = EactionState::EAS_Unoccupied;
}

FName ACPPCharacter::GetCurrentAttack()
{
    attackNumber++;
    if (attackNumber > 3) attackNumber = 1;
    return FName("Attack " + FString::FromInt(attackNumber));
}

void ACPPCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Look);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Jump);
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Interact);
        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Attack);
    }
}

void ACPPCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
    if (equippedWeapon && equippedWeapon->GetWeaponBoxComponent())
    {
        equippedWeapon->GetWeaponBoxComponent()->SetCollisionEnabled(CollisionEnabled);
        equippedWeapon->ignoreActors.Empty();
    }
}

void ACPPCharacter::SetOverlappingItem(AItem* item)
{
    overlappingItem = item;
}

void ACPPCharacter::AddCurrency(ACurrency* currency)
{
    if (attributeComponent && currency)
    {
        attributeComponent->AddCurrency(currency->GetCurrency());
    }
}
