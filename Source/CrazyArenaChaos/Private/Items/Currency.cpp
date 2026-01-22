
#include "Items/Currency.h"

#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"

void ACurrency::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
    {
        IPickupInterface::Execute_AddCurrency(OtherActor, this);
        
        //~ we don't want to pick it up twice, but we can't destroy it until fx is played.
        SetActorEnableCollision(false);
        sphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
        BP_CurrencyCollected(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    }
}
