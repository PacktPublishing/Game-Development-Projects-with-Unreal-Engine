#include "PickupsPlayerController.h"
#include "Blueprint/UserWidget.h"

void APickupsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController() || ScoreboardMenuClass == nullptr)
	{
		return;		
	}

	ScoreboardMenu = CreateWidget<UUserWidget>(this, ScoreboardMenuClass);

	if (ScoreboardMenu != nullptr)
	{
		ScoreboardMenu->AddToViewport(0);
	}
}