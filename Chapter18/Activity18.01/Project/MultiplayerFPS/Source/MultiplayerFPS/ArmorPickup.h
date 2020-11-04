#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "ArmorPickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AArmorPickup : public APickup
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor Pickup")
	float ArmorAmount = 20.0f;

	virtual void OnPickedUp(class AFPSCharacter* Character) override;
};
