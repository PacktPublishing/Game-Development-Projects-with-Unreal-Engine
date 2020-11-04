// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "EnumerationsGameMode.h"
#include "EnumerationsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEnumerationsGameMode::AEnumerationsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
