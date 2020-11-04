// Fill out your copyright notice in the Description page of Project Settings.

#include "DodgeballFunctionLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"

bool UDodgeballFunctionLibrary::CanSeeActor(const UWorld* World,
											FVector Location,
											const AActor * TargetActor,
											TArray<const AActor*> IgnoreActors)
{
	if (TargetActor == nullptr) return false;

	// Store the results of the Line Trace
	FHitResult Hit;

	// Where the Line Trace starts and ends
	FVector Start = Location;
	FVector End = TargetActor->GetActorLocation();

	// The trace channel we want to compare against
	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;

	FCollisionQueryParams QueryParams;
	// Ignore the actors specified
	QueryParams.AddIgnoredActors(IgnoreActors);

	// Execute the Line Trace
	World->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);

	// Sweep Trace logic (not used, only for demonstration)
	/*
	// Rotation of the shape used in the Sweep Trace
	FQuat Rotation = FQuat::Identity;
	// Shape of the object used in the Sweep Trace
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(20.f, 20.f, 20.f));
	GetWorld()->SweepSingleByChannel(Hit,
									 Start,
									 End,
									 Rotation,
									 Channel,
									 Shape);
	*/

	// Show the Line Trace inside the game
	DrawDebugLine(World, Start, End, FColor::Red);

	return !Hit.bBlockingHit;
}
