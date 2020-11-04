// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GFInstancesGameMode.h"
#include "GFInstancesCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGFInstancesGameMode::AGFInstancesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
