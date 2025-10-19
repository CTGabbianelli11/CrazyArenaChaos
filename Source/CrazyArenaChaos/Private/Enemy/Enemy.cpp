// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "CrazyArenaChaos/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


}

void AEnemy::PlayHitReactMontage(const FName& sectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, sectionName.ToString(), false);
		AnimInstance->Montage_JumpToSection(sectionName, HitReactMontage);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}



void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::GetHit(const FVector& impactPoint)
{
	DRAW_SPHERE_COLOR(impactPoint,FColor::Orange);

	DirectionalHitReact(impactPoint);
}
void AEnemy::CharacterDied()
{
}
void AEnemy::DirectionalHitReact(const FVector& impactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector impactLowered(impactPoint.X, impactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (impactLowered - GetActorLocation()).GetSafeNormal();

	//Forard * To hit = |Forward||ToHit| * cos(theta)
	//Forward
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//take inverse cos (arccosin) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	//convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	//if cross product points down theta is negative
	FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	FName Section("FromFront");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	//Note : Uncomment when animations in montage are added
	//else if( Theta >= -135.f && Theta < 45.f)
	//{
	//	Section = FName("FromLeft");
	//}
	//else if( Theta >= 45.f && Theta < 135.f)
	//{
	//	Section = FName("FromRight");
	//}
	//else
	//{
	//	Section = FName("FromBack");
	//}

	//GetMesh()->AddImpulseAtLocation(ToHit*100000.f,GetActorLocation(),FName("Root"));

	//PlayHitReactMontage(Section);

	EnableRagdoll(ToHit);


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta), false);

	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
}
void AEnemy::EnableRagdoll(FVector hitDirection)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->Deactivate();

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->AddImpulse(( - hitDirection * 100000.f)+FVector::UpVector*10000.f);
}
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

