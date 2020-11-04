// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryBox.h"
#include "Components/BoxComponent.h"
#include "DodgeballCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

AVictoryBox::AVictoryBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(60.0f, 60.0f, 60.0f));
	CollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AVictoryBox::OnBeginOverlap);
}

void AVictoryBox::BeginPlay()
{
	Super::BeginPlay();
}

void AVictoryBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVictoryBox::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ADodgeballCharacter>(OtherActor))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(),
									   nullptr,
									   EQuitPreference::Quit,
									   true);
	}
}

