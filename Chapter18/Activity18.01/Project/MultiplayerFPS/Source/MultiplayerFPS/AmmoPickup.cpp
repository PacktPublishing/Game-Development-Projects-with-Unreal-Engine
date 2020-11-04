#include "AmmoPickup.h"
#include "FPSCharacter.h"

void AAmmoPickup::OnPickedUp(AFPSCharacter* Character)
{
	Super::OnPickedUp(Character);

	Character->AddAmmo(AmmoType, AmmoAmount);
}