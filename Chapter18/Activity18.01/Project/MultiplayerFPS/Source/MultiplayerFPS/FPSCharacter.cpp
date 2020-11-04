#include "FPSCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "MultiplayerFPSGameModeBase.h"
#include "FPSPlayerController.h"
#include "Weapon.h"

AFPSCharacter::AFPSCharacter()
{
	// Initialize the camera component and attach it to the skeletal mesh in the Camera socket

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), "Camera");

	// Set the max walk speed to 800 and the jump z velocity to 600

	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	// This character doesn’t need to tick

	PrimaryActorTick.bCanEverTick = false;
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);

	// Only the authority can go from this point on

	if (!HasAuthority())
	{
		return;		
	}

	// Initialize Health with the value of MaxHealth

	SetHealth(MaxHealth);

	// Initialize the weapons array

	const int32 WeaponCount = ENUM_TO_INT32(EWeaponType::MAX);

	Weapons.Init(nullptr, WeaponCount);

	// Initialize the ammo array

	const int32 AmmoCount = ENUM_TO_INT32(EAmmoType::MAX);

	Ammo.Init(50, AmmoCount);

	// Add all the weapons

	for (int32 i = 0; i < WeaponCount; i++)
	{
		AddWeapon((EWeaponType)i);
	}

	// Equip the machine gun to make sure there is always an equipped weapon

	EquipWeapon(EWeaponType::MachineGun, false);

	// Save the game mode

	GameMode = Cast<AMultiplayerFPSGameModeBase>(GetWorld()->GetAuthGameMode());
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the action mappings

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnPressedJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnPressedFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::OnReleasedFire);

	PlayerInputComponent->BindAction("Previous Weapon", IE_Pressed, this, &AFPSCharacter::OnPressedPreviousWeapon);
	PlayerInputComponent->BindAction("Next Weapon", IE_Pressed, this, &AFPSCharacter::OnPressedNextWeapon);

	PlayerInputComponent->BindAction("Pistol", IE_Pressed, this, &AFPSCharacter::OnPressedPistol);
	PlayerInputComponent->BindAction("Machine Gun", IE_Pressed, this, &AFPSCharacter::OnPressedMachineGun);
	PlayerInputComponent->BindAction("Railgun", IE_Pressed, this, &AFPSCharacter::OnPressedRailgun);

	PlayerInputComponent->BindAction("Scoreboard", IE_Pressed, this, &AFPSCharacter::OnPressedScoreboard);

	// Bind the axis mappings

	PlayerInputComponent->BindAxis("Move Forward", this, &AFPSCharacter::OnAxisMoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AFPSCharacter::OnAxisMoveRight);
	PlayerInputComponent->BindAxis("Look Up", this, &AFPSCharacter::OnAxisLookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::OnAxisTurn);
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPSCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Weapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Ammo, COND_OwnerOnly);
}

void AFPSCharacter::OnPressedJump()
{
	Jump();
}

void AFPSCharacter::OnPressedFire()
{
	if (Weapon != nullptr)
	{
		Weapon->OnPressedFire();
	}
}

void AFPSCharacter::OnReleasedFire()
{
	if (Weapon != nullptr)
	{
		Weapon->OnReleasedFire();
	}
}

void AFPSCharacter::OnPressedPistol()
{
	ServerEquipWeapon(EWeaponType::Pistol);
}

void AFPSCharacter::OnPressedMachineGun()
{
	ServerEquipWeapon(EWeaponType::MachineGun);
}

void AFPSCharacter::OnPressedRailgun()
{
	ServerEquipWeapon(EWeaponType::Railgun);
}

void AFPSCharacter::OnPressedPreviousWeapon()
{
	ServerCycleWeapons(-1);
}

void AFPSCharacter::OnPressedNextWeapon()
{
	ServerCycleWeapons(1);
}

void AFPSCharacter::OnPressedScoreboard()
{
	AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());

	if (PlayerController != nullptr)
	{
		PlayerController->ToggleScoreboard();
	}
}

void AFPSCharacter::OnAxisMoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AFPSCharacter::OnAxisMoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AFPSCharacter::OnAxisLookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AFPSCharacter::OnAxisTurn(float Value)
{
	AddControllerYawInput(Value);
}

void AFPSCharacter::ArmorAbsorbDamage(float& Damage)
{
	// Calculate how much damage was absorbed and subtract that from the Armor

	const float AbsorbedDamage = Damage * ArmorAbsorption;
	const float RemainingArmor = Armor - AbsorbedDamage;

	SetArmor(RemainingArmor);

	// Recalculate the damage

	Damage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmor, 0.0f);
}

void AFPSCharacter::ServerCycleWeapons_Implementation(int32 Direction)
{
	// When Direction == 1 the search will go forward and do the next weapon functionality.
	// When Direction == -1 the search will go backwards and do the previous weapon functionality.

	const int32 WeaponCount = Weapons.Num();

	// Cycle through each index in the weapons array and try to equip it

	const int32 StartWeaponIndex = GET_CIRCULAR_ARRAY_INDEX(WeaponIndex + Direction, WeaponCount);

	for (int32 i = StartWeaponIndex; i != WeaponIndex; i = GET_CIRCULAR_ARRAY_INDEX(i + Direction, WeaponCount))
	{
		// Abort the for loop if the equip was successful

		if (EquipWeapon((EWeaponType)i))
		{
			break;
		}
	}
}

void AFPSCharacter::ServerEquipWeapon_Implementation(EWeaponType WeaponType)
{
	EquipWeapon(WeaponType);
}

bool AFPSCharacter::EquipWeapon(EWeaponType WeaponType, bool bPlaySound)
{
	// Validate the equip

	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);

	if (!Weapons.IsValidIndex(NewWeaponIndex))
		return false;

	AWeapon* NewWeapon = Weapons[NewWeaponIndex];
	
	if (NewWeapon == nullptr || Weapon == NewWeapon)
		return false;

	// Unequip the current weapon

	if (Weapon != nullptr)
	{
		Weapon->SetActorHiddenInGame(true);
	}

	// Equip the new weapon

	Weapon = NewWeapon;
	WeaponIndex = NewWeaponIndex;

	Weapon->SetActorHiddenInGame(false);

	// Play weapon change sound

	if (WeaponChangeSound != nullptr && bPlaySound)
	{
		ClientPlaySound2D(WeaponChangeSound);
	}

	return true;
}

void AFPSCharacter::AddWeapon(EWeaponType WeaponType)
{
	// Validate the add

	const int32 NewWeaponIndex = ENUM_TO_INT32(WeaponType);

	if (!WeaponClasses.IsValidIndex(NewWeaponIndex) || Weapons[NewWeaponIndex] != nullptr)
		return;

	UClass* WeaponClass = WeaponClasses[NewWeaponIndex];

	if (WeaponClass == nullptr)
		return;

	// Spawn the new weapon with this character as its owner

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();

	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParameters);

	if (NewWeapon == nullptr)
		return;

	// Hide the weapon in the beginning

	NewWeapon->SetActorHiddenInGame(true);

	// Assign the new weapon to the respective index

	Weapons[NewWeaponIndex] = NewWeapon;

	// Attach the weapon to the right hand grip socket of the character

	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
}

void AFPSCharacter::MulticastPlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}

void AFPSCharacter::ClientPlaySound2D_Implementation(USoundBase* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void AFPSCharacter::ApplyDamage(float Damage, AFPSCharacter* DamageCauser)
{
	if (IsDead())
		return;

	// Deduct the armor and the health

	ArmorAbsorbDamage(Damage);

	RemoveHealth(Damage);

	// Play the hit sound on the owning client of the DamageCauser

	if (HitSound != nullptr && DamageCauser != nullptr)
	{
		DamageCauser->ClientPlaySound2D(HitSound);
	}

	if (IsDead())
	{
		if (GameMode != nullptr)
		{
			GameMode->OnKill(DamageCauser->GetController(), GetController());
		}
	}
	else
	{
		ClientPlaySound2D(PainSound);
	}
}

void AFPSCharacter::FellOutOfWorld(const UDamageType& DmgType)
{
	// We don't want to call the parent version because it will immediately destroy the actor. We want to execute the OnKill logic in the game mode.

	if (GameMode != nullptr)
	{
		GameMode->OnKill(nullptr, GetController());
	}
}

void AFPSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Destroy all of the weapons

	for (AWeapon* WeaponToDestroy : Weapons)
	{
		WeaponToDestroy->Destroy();
	}
}

void AFPSCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UGameplayStatics::PlaySound2D(GetWorld(), LandSound);
}