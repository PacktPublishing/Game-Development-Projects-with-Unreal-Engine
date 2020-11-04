#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Player Controller")
	TSubclassOf<class UPlayerMenu> PlayerMenuClass;

	UPROPERTY()
	class UPlayerMenu* PlayerMenu;

	virtual void BeginPlay() override;

public:

	void ToggleScoreboard();

	UFUNCTION(Client, Reliable)
	void ClientNotifyKill(const FString& Name);	

	UFUNCTION(Client, Reliable)
	void ClientShowScoreboard();
};