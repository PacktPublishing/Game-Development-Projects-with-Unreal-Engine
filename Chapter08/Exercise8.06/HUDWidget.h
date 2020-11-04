// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class DODGEBALL_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void UpdateHealthPercent(float HealthPercent);

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
};
