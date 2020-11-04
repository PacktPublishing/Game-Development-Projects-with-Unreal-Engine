// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SightSource = CreateDefaultSubobject<USceneComponent>(TEXT("Sight Source"));
	SightSource->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::LookAtActor(AActor * TargetActor)
{
	
}

bool AEnemyCharacter::CanSeeActor(const AActor * TargetActor) const
{
	if (TargetActor == nullptr) return; 
 
	// Store the results of the Line Trace 
	FHitResult Hit; 
 
	// Where the Line Trace starts and ends 
	FVector Start = GetActorLocation(); 
	FVector End = TargetActor->GetActorLocation(); 
 
	// The trace channel we want to compare against 
	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility; 
 
	FCollisionQueryParams QueryParams; 
	// Ignore the actor that's executing this Line Trace 
	QueryParams.AddIgnoredActor(this); 
	// Ignore the target we're checking for
	QueryParams.AddIgnoredActor(TargetActor);
 
	// Execute the Line Trace 
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams); 
 
	// Show me the Line Trace inside the game 
	DrawDebugLine(GetWorld(), Start, End, FColor::Red); 
 
	return !Hit.bBlockingHit;
}


