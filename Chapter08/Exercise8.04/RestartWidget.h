// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartWidget.generated.h"

/**
 * 
 */
UCLASS()
class DODGEBALL_API URestartWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	void OnRestartClicked();

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;
	
};
