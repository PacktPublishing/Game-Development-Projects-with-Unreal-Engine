// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableActor_Base.generated.h"

UCLASS()
class SUPERSIDESCROLLER_API APickableActor_Base : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	APickableActor_Base();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* PickupSound;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = PickableItem)
	class USphereComponent* CollisionComp;

	/** Static mesh component */
	UPROPERTY(VisibleDefaultsOnly, Category = PickableItem)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = PickableItem)
	class URotatingMovementComponent* RotationComp;

	virtual void PlayerPickedUp(class ASuperSideScroller_Player* Player);

private:

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
