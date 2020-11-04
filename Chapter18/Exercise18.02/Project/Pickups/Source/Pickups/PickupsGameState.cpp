#include "PickupsGameState.h"
#include "Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"

void APickupsGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupsGameState, PickupsRemaining);
}

void APickupsGameState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Pickups;

	UGameplayStatics::GetAllActorsOfClass(this, APickup::StaticClass(), Pickups);

	PickupsRemaining = Pickups.Num();
}

TArray<APlayerState*> APickupsGameState::GetPlayerStatesOrderedByScore() const
{
	TArray<APlayerState*> PlayerStates(PlayerArray);

	PlayerStates.Sort([](const APlayerState& A, const APlayerState& B) { return A.Score > B.Score; });

	return PlayerStates;
}