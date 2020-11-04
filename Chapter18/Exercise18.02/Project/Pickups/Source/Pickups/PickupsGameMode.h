#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PickupsGameMode.generated.h"

UCLASS(minimalapi)
class APickupsGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	class APickupsGameState* MyGameState;

	APickupsGameMode();

	virtual void BeginPlay() override;

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

	virtual bool ReadyToStartMatch_Implementation() override;
	virtual bool ReadyToEndMatch_Implementation() override;

	void RestartMap();
};