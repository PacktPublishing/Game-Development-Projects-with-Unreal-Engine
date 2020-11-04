#include "Weapon.h"
#include "FPSCharacter.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"

AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = Mesh;

	bReplicates = true;

	PrimaryActorTick.bCanEverTick = false;
}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	Character = Cast<AFPSCharacter>(NewOwner);
}

void AWeapon::StartFire()
{
	// Abort if the trigger is no longer down OR if there is still time remaining in the FireTimer

	if (!bWantsFire || GetWorldTimerManager().GetTimerRemaining(FireTimer) > 0.0f)
		return;

	// If there is no ammo, play the no ammo sound on the owning client of the owning character and abort

	if (Character->GetAmmo(AmmoType) == 0)
	{
		if (NoAmmoSound != nullptr)
		{
			Character->ClientPlaySound2D(NoAmmoSound);
		}

		return;
	}

	// Consume the ammo

	Character->ConsumeAmmo(AmmoType, 1);

	// Fire the hitscan using the camera location and direction

	const FVector FireLocation = Character->GetCameraLocation();
	const FVector FireDirection = Character->GetCameraDirection();

	FireHitscan(FireLocation, FireDirection);

	// Play the fire anim montage in all of the instances of the owning character

	if (FireAnimMontage != nullptr)
	{
		Character->MulticastPlayAnimMontage(FireAnimMontage);
	}

	// Schedule the FireTimer depending on the value of the FireMode

	if (FireMode == EWeaponFireMode::Automatic && bWantsFire)
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon::StartFire, FireRate);
	}
	else if (FireMode == EWeaponFireMode::Single)
	{
		GetWorldTimerManager().SetTimer(FireTimer, FireRate, false);
	}
}

void AWeapon::FireHitscan(FVector FireLocation, FVector FireDirection)
{
	// Perform a line trace from the FireLocation and towards the FireDirection multiplied by the HitscanRange, that ignores the owning character

	FHitResult Hit(ForceInit);
	FCollisionQueryParams TraceParams("Fire Trace", false, Character);

	const FVector Start = FireLocation;
	const FVector End = Start + FireDirection * HitscanRange;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	// Apply damage if the hit actor is a AFPSCharacter

	AFPSCharacter* HitCharacter = Cast<AFPSCharacter>(Hit.Actor.Get());

	if (HitCharacter != nullptr)
	{
		HitCharacter->ApplyDamage(HitscanDamage, Character);
	}
}

void AWeapon::ServerStartFire_Implementation()
{
	bWantsFire = true;

	StartFire();
}

void AWeapon::ServerStopFire_Implementation()
{
	bWantsFire = false;
}

void AWeapon::OnPressedFire()
{
	ServerStartFire();
}

void AWeapon::OnReleasedFire()
{
	ServerStopFire();
}