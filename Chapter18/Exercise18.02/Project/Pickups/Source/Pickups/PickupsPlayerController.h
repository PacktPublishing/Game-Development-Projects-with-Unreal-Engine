#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PickupsPlayerController.generated.h"

UCLASS()
class PICKUPS_API APickupsPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Player Controller")
	TSubclassOf<class UUserWidget> ScoreboardMenuClass;

	UPROPERTY()
	class UUserWidget* ScoreboardMenu;

	virtual void BeginPlay() override;
};