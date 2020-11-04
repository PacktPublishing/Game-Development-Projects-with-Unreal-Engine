#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMenu.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UPlayerMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleScoreboard();

	UFUNCTION(BlueprintImplementableEvent)
	void SetScoreboardVisibility(bool bIsVisible);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyKill(const FString& Name);
};
