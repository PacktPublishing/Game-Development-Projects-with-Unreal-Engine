#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OwnershipTestActor.generated.h"

UCLASS()
class OWNERSHIPROLES_API AOwnershipTestActor : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ownership Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ownership Test Actor")
	float OwnershipRadius = 400.0f;

	AOwnershipTestActor();

	virtual void Tick(float DeltaTime) override;

};