#include "Pickup.h"
#include "PickupsCharacter.h"
#include "PickupsGameState.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

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

void APickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	APickupsCharacter* Character = Cast<APickupsCharacter>(OtherActor);

	if (Character == nullptr || !HasAuthority())
	{
		return;
	}

	APickupsGameState* GameState = Cast<APickupsGameState>(GetWorld()->GetGameState());

	if (GameState != nullptr)
	{
		GameState->RemovePickup();
	}

	Character->ClientPlaySound2D(PickupSound);
	Character->AddScore(10);
	Character->AddPickup();

	Destroy();
}