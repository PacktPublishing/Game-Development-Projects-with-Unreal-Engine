// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RPCGameMode.h"
#include "RPCCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPCGameMode::ARPCGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
