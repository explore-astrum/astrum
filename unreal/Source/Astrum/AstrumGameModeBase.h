// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Interop/Connection/SpatialWorkerConnection.h"
#include "Interop/SpatialStaticComponentView.h"
#include "EngineClasses/SpatialGameInstance.h"
#include "EngineClasses/SpatialNetDriver.h"
#include "Schema/Component.h"
#include "Schema/DynamicComponent.h"
#include "Schema/StandardLibrary.h"
#include "Interop/SpatialDispatcher.h"

#include "AstrumComponentView.h"

#include "AstrumGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ASTRUM_API AAstrumGameModeBase : public AGameMode //need this for handling has started()
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY()
	USpatialNetDriver* NetDriver;

	UFUNCTION()
	void GetProcessOps(struct FOpList OpList);

	UPROPERTY()
	UAstrumComponentView* ComponentView;

	virtual void HandleMatchHasStarted() override;
	
};
