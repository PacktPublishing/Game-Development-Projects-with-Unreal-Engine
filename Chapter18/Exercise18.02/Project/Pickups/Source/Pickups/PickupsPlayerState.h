#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PickupsPlayerState.generated.h"

UCLASS()
class PICKUPS_API APickupsPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Pickups;

public:

	void AddPickup() { Pickups++; }
};
