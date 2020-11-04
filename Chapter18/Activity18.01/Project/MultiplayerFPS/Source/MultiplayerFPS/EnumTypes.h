#pragma once

UENUM()
enum class EWeaponType : uint8
{
	Pistol,
	MachineGun,
	Railgun,
	MAX
};

UENUM()
enum class EWeaponFireMode : uint8
{
	Single,
	Automatic
};

UENUM()
enum class EAmmoType : uint8
{
	PistolBullets,
	MachineGunBullets,
	Slugs,
	MAX
};