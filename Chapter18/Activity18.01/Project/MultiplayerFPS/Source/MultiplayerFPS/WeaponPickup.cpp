#include "WeaponPickup.h"
#include "FPSCharacter.h"

void AWeaponPickup::OnPickedUp(AFPSCharacter* Character)
{
	Super::OnPickedUp(Character);

	Character->AddWeapon(WeaponType);
	Character->AddAmmo((EAmmoType)WeaponType, AmmoAmount);
}