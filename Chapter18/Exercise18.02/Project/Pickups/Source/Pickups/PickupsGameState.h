#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PickupsGameState.generated.h"

UCLASS()
class PICKUPS_API APickupsGameState : public AGameState
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 PickupsRemaining;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	TArray<APlayerState*> GetPlayerStatesOrderedByScore() const;

public:

	void RemovePickup() { PickupsRemaining--; }
	bool HasPickups() const { return PickupsRemaining > 0; }
};
