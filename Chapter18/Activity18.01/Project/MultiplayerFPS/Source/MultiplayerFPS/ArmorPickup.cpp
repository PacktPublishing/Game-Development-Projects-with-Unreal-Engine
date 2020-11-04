#include "ArmorPickup.h"
#include "FPSCharacter.h"

void AArmorPickup::OnPickedUp(AFPSCharacter* Character)
{
	Super::OnPickedUp(Character);

	Character->AddArmor(ArmorAmount);
}