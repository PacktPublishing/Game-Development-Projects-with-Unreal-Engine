#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultiplayerFPS.h"
#include "EnumTypes.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include "FPSCharacter.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FPS Character")
	class UCameraComponent* Camera;

	// Health

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
	float Health = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	float MaxHealth = 100.0f;

	// Armor

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
	float Armor = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	float MaxArmor = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	float ArmorAbsorption = 0.5;

	// Ammo

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
	TArray<int32> Ammo;

	// Weapons

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
	TArray<AWeapon*> Weapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	TArray<TSubclassOf<AWeapon>> WeaponClasses;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
	AWeapon* Weapon;

	int32 WeaponIndex = INDEX_NONE;

	// Sounds

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* SpawnSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* WeaponChangeSound;

	// Constructor and overrided

	AFPSCharacter();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input

	void OnPressedJump();

	void OnPressedFire();
	void OnReleasedFire();

	void OnPressedPistol();
	
	void OnPressedMachineGun();

	void OnPressedRailgun();

	void OnPressedPreviousWeapon();
	void OnPressedNextWeapon();

	void OnAxisMoveForward(float Value);
	void OnAxisMoveRight(float Value);
	void OnAxisLookUp(float Value);
	void OnAxisTurn(float Value);

	// Server RPCs

	UFUNCTION(Server, Reliable)
	void ServerCycleWeapons(int32 Direction);

	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(EWeaponType WeaponType);

	// Weapons

	bool EquipWeapon(EWeaponType WeaponType, bool bPlaySound = true);

public:

	// Damage

	void ApplyDamage(float Damage, AFPSCharacter* DamageCauser);

	// RPCs

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAnimMontage(UAnimMontage* AnimMontage);

	UFUNCTION(Client, Unreliable)
	void ClientPlaySound2D(USoundBase* Sound);

	// Health

	void AddHealth(float Amount) { SetHealth(Health + Amount); }
	void RemoveHealth(float Amount) { SetHealth(Health - Amount); }
	void SetHealth(float NewHealth) { Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth); }
	bool IsDead() const { return Health == 0.0f; }

	// Armor

	void AddArmor(float Amount) { SetArmor(Armor + Amount); }
	void SetArmor(float Amount) { Armor = FMath::Clamp(Amount, 0.0f, MaxArmor); }
	void ArmorAbsorbDamage(float& Damage);

	// Weapons

	void AddWeapon(EWeaponType WeaponType);

	// Ammo

	UFUNCTION(BlueprintCallable, Category = "FPS Character")
	int32 GetWeaponAmmo() const { return Weapon != nullptr ? Ammo[ENUM_TO_INT32(Weapon->GetAmmoType())] : 0; }

	void AddAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) + Amount); }
	void ConsumeAmmo(EAmmoType AmmoType, int32 Amount) { SetAmmo(AmmoType, GetAmmo(AmmoType) - Amount); }

	int32 GetAmmo(EAmmoType AmmoType) const { return Ammo[ENUM_TO_INT32(AmmoType)]; }
	void SetAmmo(EAmmoType AmmoType, int32 Amount) { Ammo[ENUM_TO_INT32(AmmoType)] = FMath::Max(0, Amount); }

	// Camera

	FVector GetCameraLocation() const { return Camera->GetComponentLocation(); }
	FVector GetCameraDirection() const { return GetControlRotation().Vector(); }
};