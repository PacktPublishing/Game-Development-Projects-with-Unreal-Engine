// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GFInstancePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GFINSTANCES_API AGFInstancePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GF Instance Player Controller")
	TSubclassOf<UUserWidget> MenuClass;

	UPROPERTY()
	UUserWidget* Menu;

	virtual void BeginPlay() override;
};
