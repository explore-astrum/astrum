// Fill out your copyright notice in the Description page of Project Settings.

#include "AstrumPlayerController.h"
#include "SpatialNetDriver.h"
#include "EntityId.h"
#include "EntityRegistry.h"

void AAstrumPlayerController::InitPlayerState()
{
	UWorld* World = GetWorld();
	check(World);
	USpatialNetDriver* NetDriver = Cast<USpatialNetDriver>(World->GetNetDriver());
	if (NetDriver)
	{
		const FEntityId EntityId = NetDriver->GetEntityRegistry()->GetEntityIdFromActor(this);
		UE_LOG(LogTemp, Log, TEXT("PC:InitPlayerState called with entity id %d"), EntityId.ToSpatialEntityId());
		if (EntityId != 0)
		{
			// EntityId is not 0, which means that this PC has already been initialized.
			return;
		}
	}

	Super::InitPlayerState();
}
