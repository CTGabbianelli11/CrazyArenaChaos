
// WeaponListEntryData.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WeaponListEntryData.generated.h"

class UTexture2D;

UCLASS(BlueprintType)
class CRAZYARENACHAOS_API UWeaponListEntryData : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 BasePrice = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bOwned = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bEquipped = false;
};