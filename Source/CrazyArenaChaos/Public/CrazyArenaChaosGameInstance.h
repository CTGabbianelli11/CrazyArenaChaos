
// CrazyArenaChaosGameInstance.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs/PersistentDataStruct.h"
#include "CrazyArenaChaosGameInstance.generated.h"

class AEnemy;

/** Broadcast when all enemies in the wave are defeated */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllEnemiesDefeated);

/** Bradcast when an enemy is removed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyRemoved);

UCLASS()
class CRAZYARENACHAOS_API UCrazyArenaChaosGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // -------------------- Persisted Player State --------------------
    UPROPERTY(BlueprintReadWrite, Category = "Player")
    FStructPersistentData playerPersistingAttributes{};

    // -------------------- Wave Tracking --------------------
    UPROPERTY(BlueprintReadOnly, Category = "Wave Attributes")
    TArray<AEnemy*> EnemiesToBeDefeated;
	
	UPROPERTY(BlueprintReadWrite, Category = "Wave Attributes")
	FWaveInfo WaveInfo {};

    // -------------------- Level Transition --------------------
    UPROPERTY(BlueprintReadWrite)
    TSubclassOf<AActor> LevelTransitionDoor;

    UPROPERTY(BlueprintReadOnly, Category = "Level Transition Attributes")
    TSoftObjectPtr<UWorld> ShopLevel;

    // -------------------- Shop Reference --------------------
    /**
     * Weak reference to the existing BP_Shop actor.
     * Register it from BP_Shop (e.g., in BeginPlay) by calling RegisterShopActor(self).
     */
    UPROPERTY(BlueprintReadOnly, Category = "Shop")
    TWeakObjectPtr<UActorComponent> ShopActor;

public:
    // -------------------- Lifecycle --------------------
    virtual void Init() override;

    // -------------------- Persistent Data Utilities --------------------
    /** Resets the persistent data to initial values. */
    UFUNCTION(BlueprintCallable, Category = "Persistent Data")
    void ResetAttributes();
    UFUNCTION(BlueprintCallable, Category = "Persistent Data")
    void ResetEnemies();

    // -------------------- Shop Registration & Access --------------------
    /** Called by BP_Shop at runtime to register itself with the GameInstance. */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    void RegisterShopActor(UActorComponent* InShop);

    /** Clears the registered Shop actor reference (optional helper). */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    void UnregisterShopActor();

    /** Returns the registered Shop actor (BP_Shop) if available, else nullptr. */
    UFUNCTION(BlueprintPure, Category = "Shop")
    UActorComponent* GetShopActor() const { return ShopActor.Get(); }

    /** Returns true if a Shop actor is currently registered. */
    UFUNCTION(BlueprintPure, Category = "Shop")
    bool HasShop() const { return ShopActor.IsValid(); }

    // -------------------- Wave Management --------------------
    UFUNCTION(BlueprintCallable) void AddEnemy(AEnemy* Enemy);
    UFUNCTION(BlueprintCallable) void RemoveEnemy(AEnemy* Enemy);

    // -------------------- Level Transitions --------------------
    UFUNCTION(BlueprintCallable) void LoadShop();
    UFUNCTION(BlueprintCallable) void RestartLevel();

public:
    // -------------------- Events --------------------
    UPROPERTY(BlueprintAssignable)
    FAllEnemiesDefeated AllEnemiesDefeated;

    UFUNCTION(BlueprintImplementableEvent)
    void AllEnemiesDefeatedEvent();

    UPROPERTY(BlueprintAssignable)
    FEnemyRemoved EnemyRemoved;

    UFUNCTION(BlueprintImplementableEvent)
    void EnemyRemovedEvent();

    UFUNCTION(BlueprintImplementableEvent)
    void PlayerDiedEvent();

private:
    FStructPersistentData DefaultPlayerAttributes{};
};
