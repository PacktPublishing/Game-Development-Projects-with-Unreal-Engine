#include "GFInstancePlayerController.h"
#include "Blueprint/UserWidget.h"

void AGFInstancePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController() || MenuClass == nullptr)
	{
		return;		
	}

	Menu = CreateWidget<UUserWidget>(this, MenuClass);

	if (Menu != nullptr)
	{
		Menu->AddToViewport(0);
	}
}