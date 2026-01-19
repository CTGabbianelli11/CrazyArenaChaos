
#include "Items/Currency.h"
#include "Interfaces/PickupInterface.h"

void ACurrency::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
    {
        IPickupInterface::Execute_AddCurrency(OtherActor, this);
        Destroy();
    }
}
