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
#include "TCPConnection.h"
#include "AstrumPlayerController.h"

#include "AstrumComponentView.h"

#include "SpawnableActor.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/StreamableManager.h"

#include "AstrumGameModeBase.generated.h"

USTRUCT()
struct FPlot {
	GENERATED_BODY()
	UPROPERTY()
	FVector2D coords;
	UPROPERTY()
	FLand land;
	UPROPERTY()
	FString owner;
	UPROPERTY()
	FString id;
};

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

	UFUNCTION()
	void SpawnActor(int relic_type, FString relic_key);

	UFUNCTION()
	void MoveRelic(FString relic_key, FVector location);

	UFUNCTION()
	void AsyncSpawn();

	UFUNCTION()
	void ChangeRelicOwner(FString relic_key, FString relic_owner);

	UFUNCTION()
	void UpdateRelicLocation(FString relic_key, FVector location);

	UFUNCTION()
	void AddPlot(FString plot_id, FVector2D coords);

	UFUNCTION()
	void ChangePlotOwner(FString plot_id, FString owner_id);

	UPROPERTY()
	UAstrumComponentView* ComponentView;

	virtual void HandleMatchHasStarted() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	ATCPConnection* tcpConnection;

	TArray<ASpawnableActor*> all_relics;

	UObjectLibrary* ObjectLibrary;
	TArray<FAssetData> actors_to_spawn;
	TArray<FPlot> all_plots;

	float land_size = 5000.0;
};
