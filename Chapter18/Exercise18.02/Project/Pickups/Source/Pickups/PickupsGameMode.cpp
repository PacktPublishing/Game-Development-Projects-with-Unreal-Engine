// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PickupsGameMode.h"
#include "PickupsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "PickupsGameState.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Engine/Engine.h"

APickupsGameMode::APickupsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void APickupsGameMode::BeginPlay()
{
	Super::BeginPlay();

	MyGameState = GetGameState<APickupsGameState>();
}

bool APickupsGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void APickupsGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "The game has started!");
}

void APickupsGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "The game has ended!");

	TArray<AActor*> Characters;

	UGameplayStatics::GetAllActorsOfClass(this, APickupsCharacter::StaticClass(), Characters);

	for (AActor* Character : Characters)
	{
		Character->Destroy();
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APickupsGameMode::RestartMap, 5.0f);
}

bool APickupsGameMode::ReadyToStartMatch_Implementation()
{
	return true;
}

bool APickupsGameMode::ReadyToEndMatch_Implementation()
{
	return MyGameState != nullptr && !MyGameState->HasPickups();
}

void APickupsGameMode::RestartMap()
{
	GetWorld()->ServerTravel(GetWorld()->GetName(), false, false);
}