// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CPPCharacter.h"
#include "Animation/AnimMontage.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include <EnhancedInputComponent.h>
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"

ACPPCharacter::ACPPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);

	//hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	//hair->SetupAttachment(GetMesh());
	//hair->AttachmentName = FString("head");

	//eyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eye Brows"));
	//eyeBrows->SetupAttachment(GetMesh());
	//eyeBrows->AttachmentName = FString("head");


}

void ACPPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterInputMappingContext, 0);
		}
	}
}

void ACPPCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D movementVector = Value.Get<FVector2D>();

	const FRotator controlRotation = GetControlRotation();
	const FRotator YawRotation(0.f,controlRotation.Yaw,0.f);

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

void ACPPCharacter::Interact(const FInputActionValue& Value)
{
	AWeapon* overlappingWeapon = Cast<AWeapon>(overlappingItem);
	if (overlappingWeapon) 
	{
		overlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		equippedWeapon = overlappingWeapon;
		state = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
}

void ACPPCharacter::Attack(const FInputActionValue& Value)
{
	if (CanAttack())
	{
		PlayAttackMontage();
		actionState = EactionState::EAS_Attacking;
	}

}
bool ACPPCharacter::CanAttack()
{
	return actionState == EactionState::EAS_Unoccupied &&
		state != ECharacterState::ECS_Unequipped;
}

void ACPPCharacter::PlayAttackMontage()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);

		const int32 selection = FMath::RandRange(0, 1);
		FName sectionName = FName();

		switch (selection)
		{
		case 0:
			sectionName = "Attack 1";
			break;
		case 1:
			sectionName = "Attack 2";
			break;
		default:
			sectionName = "Attack 1";
			break;
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, sectionName.ToString(), false);
		AnimInstance->Montage_JumpToSection(sectionName, AttackMontage);
	}
}

void ACPPCharacter::AttackEnd()
{
	actionState = EactionState::EAS_Unoccupied;
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
		equippedWeapon->GetWeaponBoxComponent()->SetCollisionEnabled(CollisionEnabled);
}

