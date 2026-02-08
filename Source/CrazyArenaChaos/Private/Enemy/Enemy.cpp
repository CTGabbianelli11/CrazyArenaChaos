// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/CrowdExcitementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "CrazyArenaChaos/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Currency.h"
#include "Animation/AnimMontage.h"
#include "CrazyArenaChaosGameInstance.h"
#include "Components/AttributeComponent.h"
#include "NiagaraComponent.h"
#include "Components/AC_HitStop.h"
#include "NiagaraFunctionLibrary.h"



AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	hitStopComponent = CreateDefaultSubobject<UAC_HitStop>(TEXT("HitStop"));

	attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	CrowdExcitementComponent = CreateDefaultSubobject<UCrowdExcitementComponent>(TEXT("Crowd Excitement"));
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

void AEnemy::GetHit(const FVector& impactPoint, const FVector& impactDirection)
{
	CrowdExcitementComponent->OnHit();

	DRAW_SPHERE_COLOR(impactPoint,FColor::Orange);

	const FVector impactLowered(impactPoint.X, impactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (impactLowered - GetActorLocation()).GetSafeNormal();

	if (hitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, hitSound, GetActorLocation(),1,1,.1f);
	}

	if (HitSystem)
	{
		const UWorld* World = GetWorld();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, HitSystem, impactPoint,ToHit.Rotation());
	}
	OnCharacterHit();

	if (attributes && attributes->IsAlive())
	{
		hitStopComponent->BeginHitStop(.2f,0,30,20,true);
		DirectionalHitReact(impactPoint,ToHit);
	}
	else if (attributes)
	{
		CharacterDied();
	}


}
void AEnemy::CharacterDied()
{
	DetachFromControllerPendingDestroy();

	GetMesh()->SetAnimInstanceClass(nullptr);



	EnableRagdoll();

	if (UCrazyArenaChaosGameInstance* instance = Cast<UCrazyArenaChaosGameInstance>(GetGameInstance()))
	{
		instance->RemoveEnemy(this);
	}
	
	DropCurrency();

	OnCharacterDied();

	CrowdExcitementComponent->OnKill();
}
void AEnemy::DropCurrency()
{
	UWorld* World = GetWorld();
	if (World && CurrencyToDrop)
	{
		FVector location = GetActorLocation();
		location.Z += 25.f;
		World->SpawnActor<ACurrency>(CurrencyToDrop, location, GetActorRotation());
	}
}
void AEnemy::DirectionalHitReact(const FVector& impactPoint,const FVector impactDirection)
{


	const FVector Forward = GetActorForwardVector();


	//Forard * To hit = |Forward||ToHit| * cos(theta)
	//Forward
	const double CosTheta = FVector::DotProduct(Forward, impactDirection);
	//take inverse cos (arccosin) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	//convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	//if cross product points down theta is negative
	FVector CrossProduct = FVector::CrossProduct(Forward, impactDirection);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	FName Section("FromLeft");
	//if (Theta >= -45.f && Theta < 45.f)
	//{
		//Section = FName("FromFront");
	//}
	//Note : Uncomment when animations in montage are added
	if( Theta >= -135.f && Theta < 45.f)
	{
		Section = FName("FromRight");
	}
	//else if( Theta >= 45.f && Theta < 135.f)
	//{
	//	Section = FName("FromRight");
	//}
	//else
	//{
	//	Section = FName("FromBack");
	//}

	//GetMesh()->AddImpulseAtLocation(ToHit*100000.f,GetActorLocation(),FName("Root"));

	PlayHitReactMontage(Section);

	//NOTE: Move to health logic later



	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta), false);

	//}
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
}
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (attributes)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Example text that prints a float: %f"), DamageAmount), false);

		attributes->RecieveDamage(DamageAmount);
	}
	return DamageAmount;
}
void AEnemy::EnableRagdoll()
{
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	UWorld* world = GetWorld();
	if (world)
	{
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(world,0);


	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->AddImpulse((character->GetActorForwardVector() * 200000.f)+FVector::UpVector*20000.f);
	}

}
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

