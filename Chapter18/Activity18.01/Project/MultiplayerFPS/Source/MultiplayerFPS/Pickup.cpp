#include "Pickup.h"
#include "FPSCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Engine/Public/TimerManager.h"

APickup::APickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName("OverlapAll");
	RootComponent = Mesh;

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>("Rotating Movement");
	RotatingMovement->RotationRate = FRotator(0.0, 90.0f, 0);

	bReplicates = true;

	PrimaryActorTick.bCanEverTick = false;
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
}

void APickup::OnPickedUp(AFPSCharacter* Character)
{
	if (PickupSound != nullptr)
	{
		Character->ClientPlaySound2D(PickupSound);
	}
}

void APickup::SetIsEnabled(bool NewbIsEnabled)
{
	bIsEnabled = NewbIsEnabled;

	SetActorHiddenInGame(!bIsEnabled);
	SetActorEnableCollision(bIsEnabled);
}

void APickup::Respawn()
{
	SetIsEnabled(true);
}

void APickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);

	// Abort if the overlapping actor is not a character or it's already dead or it's not authority

	if (Character == nullptr || Character->IsDead() || !HasAuthority())
	{
		return;		
	}

	// Execute the custom pick up logic

	OnPickedUp(Character);

	// Disable the pickup

	SetIsEnabled(false);

	// Start the timer to respawn the pickup

	GetWorldTimerManager().SetTimer(RespawnTimer, this, &APickup::Respawn, RespawnTime);
}