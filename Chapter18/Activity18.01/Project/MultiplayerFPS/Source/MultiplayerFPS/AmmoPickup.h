#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "EnumTypes.h"
#include "AmmoPickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Pickup")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Pickup")
	int32 AmmoAmount;

	virtual void OnPickedUp(class AFPSCharacter* Character) override;
};
