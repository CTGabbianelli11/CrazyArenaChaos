#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Currency.generated.h"

UCLASS()
class CRAZYARENACHAOS_API ACurrency : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/** NOTE (cody) I noticed there's an int32 currencyValue in the cpp *AND* BP_Currency has an int64 CurrencyAmount. I'm not 100% but these may be intended for the same thing. If not, delete this comment. */
	UPROPERTY(EditAnywhere, Category = "Currency Properties")
	int32 currencyValue;

	/** Use this function to play any fx when currency is collected. Remember to destroy the actor afterward.  */
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "Currency Collected"))
	void BP_CurrencyCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FORCEINLINE int32 GetCurrency() { return currencyValue; }
};
