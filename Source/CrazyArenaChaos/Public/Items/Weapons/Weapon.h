
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
class USceneComponent;

/**
 * 
 */
UCLASS()
class CRAZYARENACHAOS_API AWeapon : public AItem
{
    GENERATED_BODY()

public:
    AWeapon();

    /** Attach this weapon to a socket on a parent component */
    void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

    /** Uniformly scale the whole weapon (meshes, colliders, trace points) */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Scale")
    void ScaleWeapon(float ScaleFactor);

    /** (Optional) Fine-grained scaling if you prefer explicit collider sizing */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Scale")
    void ScaleWeaponExplicit(float ScaleFactor);

    /** Runtime list used to avoid re-hitting the same actors while tracing */
    TArray<AActor*> ignoreActors;

protected:
    virtual void BeginPlay() override;

    /** Inherited from AItem — we still want sphere overlap behavior */
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult) override;

    virtual void OnEndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

    UFUNCTION()
    void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                      const FHitResult& SweepResult);

    /** Ensure hierarchy is correct both in editor and at runtime */
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void PostInitializeComponents() override;

private:
    /** Parent we can scale once to affect everything else */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components", meta = (AllowPrivateAccess="true"))
    USceneComponent* ScaleContainer = nullptr;

    /** Collision used for hit detection while swinging */
    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
    UBoxComponent* WeaponBoxComponent;

    /** Trace start anchor (moves/scales with the weapon) */
    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceStart;

    /** Trace end anchor (moves/scales with the weapon) */
    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceEnd;

    /** Base damage for ApplyDamage() */
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
    float damage = 20.f;

    /*
     SFX
    */
    UPROPERTY(EditDefaultsOnly, Category = "Sound Effect")
    USoundWave* HitSound;
    
    UPROPERTY(EditDefaultsOnly, Category = "Sound Effect")
    USoundWave* MissSoundEffect;

    /** Re-attach parts under ScaleContainer while keeping relative transforms */
    void EnsurePartsAttachedToScaleContainer();

public:
    FORCEINLINE UBoxComponent* GetWeaponBoxComponent() const { return WeaponBoxComponent; }
    /** (Optional) expose read-only access to the scale container if needed in BP */
    FORCEINLINE USceneComponent* GetScaleContainer() const { return ScaleContainer; }
};
