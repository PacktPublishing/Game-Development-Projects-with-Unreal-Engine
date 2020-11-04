#include "FPSCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AFPSCharacter::AFPSCharacter()
{
	// Initialize the camera component and attach it to the skeletal mesh in the Camera socket

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), "Camera");

	// Set the max walk speed to 800 and the jump z velocity to 600

	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	// This character doesn't need to tick

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
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the action mappings

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnPressedJump);

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
}

void AFPSCharacter::OnPressedJump()
{
	Jump();
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
