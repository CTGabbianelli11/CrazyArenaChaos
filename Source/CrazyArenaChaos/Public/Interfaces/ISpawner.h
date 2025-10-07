#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISpawner.generated.h"  

UINTERFACE(BlueprintType)
class CRAZYARENACHAOS_API USpawner : public UInterface
{
    GENERATED_BODY()
};

class CRAZYARENACHAOS_API ISpawner
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawner")
    void Spawn();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawner")
    void ResetSpawn();
};