#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AHealthPickup : public APickup
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Pickup")
	float HealAmount = 20.0f;

	virtual void OnPickedUp(class AFPSCharacter* Character) override;
};
