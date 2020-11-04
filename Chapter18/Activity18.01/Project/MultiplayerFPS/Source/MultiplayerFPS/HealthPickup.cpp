#include "HealthPickup.h"
#include "FPSCharacter.h"

void AHealthPickup::OnPickedUp(AFPSCharacter* Character)
{
	Super::OnPickedUp(Character);

	Character->AddHealth(HealAmount);
}