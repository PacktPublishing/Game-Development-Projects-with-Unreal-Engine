#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FPSGameState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSGameState : public AGameState
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Game State")
	int32 KillLimit;

public:

	UFUNCTION(BlueprintCallable)
	TArray<class AFPSPlayerState*> GetPlayerStatesOrderedByKills() const;

	void SetKillLimit(int32 NewKillLimit) { KillLimit = NewKillLimit; }
};
