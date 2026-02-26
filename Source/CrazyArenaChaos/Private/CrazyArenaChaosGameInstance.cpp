
// CrazyArenaChaosGameInstance.cpp

#include "CrazyArenaChaosGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"

void UCrazyArenaChaosGameInstance::Init()
{
    Super::Init();
    // Snapshot defaults so ResetAttributes can restore them later
    DefaultPlayerAttributes = playerPersistingAttributes;
}

void UCrazyArenaChaosGameInstance::ResetAttributes()
{
    playerPersistingAttributes = DefaultPlayerAttributes;
}

void UCrazyArenaChaosGameInstance::ResetEnemies()
{
    EnemiesToBeDefeated.Empty();
}

void UCrazyArenaChaosGameInstance::RegisterShopActor(UActorComponent* InShop)
{
    if (IsValid(InShop))
    {
        ShopActor = InShop;
    }
}

void UCrazyArenaChaosGameInstance::UnregisterShopActor()
{
    ShopActor.Reset();
}

void UCrazyArenaChaosGameInstance::AddEnemy(AEnemy* Enemy)
{
    if (Enemy)
    {
        EnemiesToBeDefeated.Add(Enemy);
    }
}

void UCrazyArenaChaosGameInstance::RemoveEnemy(AEnemy* Enemy)
{
    if (EnemiesToBeDefeated.Contains(Enemy))
    {
        EnemiesToBeDefeated.Remove(Enemy);
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, TEXT("Enemy not found in list"));
        }
    }

    if (EnemiesToBeDefeated.Num() <= 0)
    {
        AllEnemiesDefeated.Broadcast();
        AllEnemiesDefeatedEvent();
    }
}

void UCrazyArenaChaosGameInstance::LoadShop()
{
    if (UWorld* World = GetWorld())
    {
        UGameplayStatics::OpenLevelBySoftObjectPtr(World, ShopLevel);
    }
}

void UCrazyArenaChaosGameInstance::RestartLevel()
{
    if (UWorld* World = GetWorld())
    {
        UGameplayStatics::OpenLevel(this, FName(World->GetName()), /*bAbsolute=*/false);
    }
}
