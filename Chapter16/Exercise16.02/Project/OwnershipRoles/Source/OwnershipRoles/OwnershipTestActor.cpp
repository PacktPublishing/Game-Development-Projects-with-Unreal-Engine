#include "OwnershipTestActor.h"
#include "DrawDebugHelpers.h"
#include "OwnershipRoles.h"
#include "OwnershipRolesCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AOwnershipTestActor::AOwnershipTestActor()
{
	// Create the static mesh component and set it as the root component

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	// We need to set bReplicates to true in order to tell Unreal Engine that this actor replicates

	bReplicates = true;

	// This actor can run the tick function

	PrimaryActorTick.bCanEverTick = true;
}

void AOwnershipTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Draw a debug sphere to show the ownership radius

	DrawDebugSphere(GetWorld(), GetActorLocation(), OwnershipRadius, 32, FColor::Yellow);

	// Only the authority should run this logic

	if (HasAuthority())
	{
		// Determine the closest AOwnershipRolesCharacter within the ownership radius

		AActor* NextOwner = nullptr;

		float MinDistance = OwnershipRadius;

		TArray<AActor*> Actors;

		UGameplayStatics::GetAllActorsOfClass(this, AOwnershipRolesCharacter::StaticClass(), Actors);

		for (AActor* Actor : Actors)
		{
			const float Distance = GetDistanceTo(Actor);

			if (Distance <= MinDistance)
			{
				MinDistance = Distance;
				NextOwner = Actor;
			}
		}

		// Set the next owner if it's different from the current one

		if (GetOwner() != NextOwner)
		{
			SetOwner(NextOwner);
		}
	}

	// Convert the local role, remote role, owner and connection into a printable string format

	const FString LocalRoleString = ROLE_TO_STRING(GetLocalRole());
	const FString RemoteRoleString = ROLE_TO_STRING(GetRemoteRole());
	const FString OwnerString = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	const FString ConnectionString = GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection");

	// Print the values so we can compare the values between the clients 

	const FString Values = FString::Printf(TEXT("LocalRole = %s\nRemoteRole = %s\nOwner = %s\nConnection = %s"), *LocalRoleString, *RemoteRoleString, *OwnerString, *ConnectionString);

	DrawDebugString(GetWorld(), GetActorLocation(), Values, nullptr, FColor::White, 0.0f, true);
}