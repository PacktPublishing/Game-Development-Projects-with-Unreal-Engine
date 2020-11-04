// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "EnumerationsCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AEnumerationsCharacter

AEnumerationsCharacter::AEnumerationsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEnumerationsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEnumerationsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEnumerationsCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AEnumerationsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEnumerationsCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AEnumerationsCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AEnumerationsCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AEnumerationsCharacter::OnResetVR);

	// Weapon inputs

	PlayerInputComponent->BindAction("Pistol", IE_Pressed, this, &AEnumerationsCharacter::OnPressedPistol);
	
	PlayerInputComponent->BindAction("Shotgun", IE_Pressed, this, &AEnumerationsCharacter::OnPressedShotgun);
	
	PlayerInputComponent->BindAction("Rocket Launcher", IE_Pressed, this, &AEnumerationsCharacter::OnPressedRocketLauncher);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AEnumerationsCharacter::OnPressedFire);
	
	PlayerInputComponent->BindAction("Previous Weapon", IE_Pressed, this, &AEnumerationsCharacter::OnPressedPreviousWeapon);

	PlayerInputComponent->BindAction("Next Weapon", IE_Pressed, this, &AEnumerationsCharacter::OnPressedNextWeapon);
}


void AEnumerationsCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AEnumerationsCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AEnumerationsCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AEnumerationsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEnumerationsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AEnumerationsCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEnumerationsCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AEnumerationsCharacter::BeginPlay()
{
	Super::BeginPlay();

	const int32 AmmoCount = ENUM_TO_INT32(EAmmoType::MAX);

	Ammo.Init(10, AmmoCount);
}

void AEnumerationsCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const int32 WeaponIndex = ENUM_TO_INT32(Weapon);
	const FString WeaponString = ENUM_TO_FSTRING("EWeaponType", Weapon);

	const FString AmmoTypeString = ENUM_TO_FSTRING("EAmmoType", Weapon);

	const int32 AmmoCount = Ammo[WeaponIndex];

	const FString String = FString::Printf(TEXT("Weapon = %s\nAmmo Type = %s\nAmmo Count = %d"), *WeaponString, *AmmoTypeString, AmmoCount);

	DrawDebugString(GetWorld(), GetActorLocation(), String, nullptr, FColor::White, 0.0f, true);
}

void AEnumerationsCharacter::OnPressedPistol()
{
	Weapon = EWeaponType::Pistol;
}

void AEnumerationsCharacter::OnPressedShotgun()
{
	Weapon = EWeaponType::Shotgun;
}

void AEnumerationsCharacter::OnPressedRocketLauncher()
{
	Weapon = EWeaponType::RocketLauncher;
}

void AEnumerationsCharacter::OnPressedFire()
{
	const int32 WeaponIndex = ENUM_TO_INT32(Weapon);
	const int32 NewRawAmmoCount = Ammo[WeaponIndex] - 1;
	const int32 NewAmmoCount = FMath::Max(NewRawAmmoCount, 0);

	Ammo[WeaponIndex] = NewAmmoCount;
}

void AEnumerationsCharacter::OnPressedPreviousWeapon()
{
	CycleWeapons(-1);
}

void AEnumerationsCharacter::OnPressedNextWeapon()
{
	CycleWeapons(1);
}

void AEnumerationsCharacter::CycleWeapons(int32 Direction)
{
	const int32 WeaponIndex = ENUM_TO_INT32(Weapon);
	const int32 AmmoCount = Ammo.Num();

	const int32 NextRawWeaponIndex = WeaponIndex + Direction;
	const int32 NextWeaponIndex = GET_CIRCULAR_ARRAY_INDEX(NextRawWeaponIndex, AmmoCount);

	Weapon = (EWeaponType)NextWeaponIndex;
}