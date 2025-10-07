#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Spawnable.generated.h"  

class AActor; 

UINTERFACE(BlueprintType)
class CRAZYARENACHAOS_API USpawnable : public UInterface
{
    GENERATED_BODY()
};

class CRAZYARENACHAOS_API ISpawnable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawn")
    void OnSpawnedFromSpawner(AActor* Spawner);
};