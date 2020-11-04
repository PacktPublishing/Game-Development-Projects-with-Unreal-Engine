// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor_Base.h"
#include "PickableActor_Powerup.generated.h"

/**
 * 
 */
UCLASS()
class SUPERSIDESCROLLER_API APickableActor_Powerup : public APickableActor_Base
{
	GENERATED_BODY()
	
protected:

	virtual void PlayerPickedUp(class ASuperSideScroller_Player* Player) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
