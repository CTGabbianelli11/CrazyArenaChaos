
// PickupInterface.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

UINTERFACE(BlueprintType)
class CRAZYARENACHAOS_API UPickupInterface : public UInterface
{
    GENERATED_BODY()
};

class CRAZYARENACHAOS_API IPickupInterface
{
    GENERATED_BODY()

public:
    /** BP + C++ overridable. Implement _Implementation in implementing classes. */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup")
    void AddCurrency(class ACurrency* Currency);

    /** BP + C++ overridable. Implement _Implementation in implementing classes. */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup")
    void SetOverlappingItem(class AItem* Item);
};
