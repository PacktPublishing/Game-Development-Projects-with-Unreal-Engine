// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuperSideScrollerCharacter.h"
#include "SuperSideScroller_Player.generated.h"

/**
 * 
 */

UCLASS()
class SUPERSIDESCROLLER_API ASuperSideScroller_Player : public ASuperSideScrollerCharacter
{
	GENERATED_BODY()

public:
	//Constructor
	ASuperSideScroller_Player();

protected:
	//Override base character class function to setup our player input component
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//Sprinting
	void Sprint();
	//StopSprinting
	void StopSprinting();

public:

	//ThrowProjectile
	void ThrowProjectile();


private:
	//Bool to control if we are sprinting. Failsafe.
	bool bIsSprinting;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* ThrowMontage;
};
