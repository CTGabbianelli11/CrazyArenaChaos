// Fill out your copyright notice in the Description page of Project Settings.
#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"

#include <EnhancedInputComponent.h>

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	capsule->SetCapsuleHalfHeight(20.f);
	capsule->SetCapsuleRadius(15.f);
	RootComponent =capsule;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bird Mesh"));

	skeletalMesh->SetupAttachment(RootComponent);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 300.f;

	viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	viewCamera->SetupAttachment(springArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingInputContext, 0);
		}
	}
}
void ABird::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, DirectionValue);
}
void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D lookValue = Value.Get<FVector2D>();
	AddControllerYawInput(lookValue.X);
	AddControllerPitchInput(lookValue.Y);
}
// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}


	//PlayerInputComponent->BindAxis(TEXT("MoveForward"),this, &ABird::MoveForward);
	//PlayerInputComponent->BindAxis(TEXT("Turn"),this, &ABird::Turn);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"),this, &ABird::LookUp);
}

