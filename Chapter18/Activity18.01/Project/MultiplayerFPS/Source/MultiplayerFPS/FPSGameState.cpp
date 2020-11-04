#include "FPSGameState.h"
#include "Net/UnrealNetwork.h"
#include "FPSPlayerState.h"

void AFPSGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSGameState, KillLimit);
}

TArray<AFPSPlayerState*> AFPSGameState::GetPlayerStatesOrderedByKills() const
{
	TArray<AFPSPlayerState*> PlayerStates;

	for (APlayerState* PlayerState : PlayerArray)
	{
		AFPSPlayerState* FPSPlayerState = Cast<AFPSPlayerState>(PlayerState);

		PlayerStates.Add(FPSPlayerState);
	}

	PlayerStates.Sort([](const AFPSPlayerState& A, const AFPSPlayerState& B) { return A.GetKills() > B.GetKills(); });

	return PlayerStates;
}