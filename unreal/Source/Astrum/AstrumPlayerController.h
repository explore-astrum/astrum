// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Interop/Connection/SpatialWorkerConnection.h"
#include "Interop/SpatialStaticComponentView.h"
#include "EngineClasses/SpatialGameInstance.h"
#include "EngineClasses/SpatialNetDriver.h"
#include "Schema/Component.h"
#include "Schema/DynamicComponent.h"
#include "Schema/StandardLibrary.h"
#include "Interop/SpatialDispatcher.h"

#include <WorkerSDK/improbable/c_schema.h>
#include <WorkerSDK/improbable/c_worker.h>

#include "AstrumComponentView.h"

#include "AstrumPlayerController.generated.h"

/**
 * 
 */

struct Worker_OpList;

USTRUCT(BlueprintType)
struct FLand {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D min;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D max;
};

UCLASS(SpatialType)
class ASTRUM_API AAstrumPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere)
		FString userID = "1";

		UFUNCTION(BlueprintCallable)
		FString GetUserID();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "properties")
		TArray<FLand> properties;

		UFUNCTION(BlueprintCallable)
		void SetProperties();

		UFUNCTION(BlueprintCallable)
		TArray<FLand> GetProperties();

		UFUNCTION(BlueprintCallable)
		void GoToServer();

		UPROPERTY()
		USpatialNetDriver* NetDriver;

		virtual void BeginPlay() override;
		//virtual void Tick(float DeltaTime) override;

		UFUNCTION()
		void GetProcessOps(struct FOpList OpList);

		UPROPERTY()
		UAstrumComponentView* ComponentView;
	
	
	
	
};
