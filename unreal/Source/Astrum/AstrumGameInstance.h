// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpatialGameInstance.h"
#include "AstrumGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ASTRUM_API UAstrumGameInstance : public USpatialGameInstance
{
	GENERATED_BODY()
	
	public:
		virtual void Init() override;

		UFUNCTION()
		virtual void BeginLoadingScreen(const FString& MapName);
		UFUNCTION()
		virtual void EndLoadingScreen(UWorld* InLoadedWorld);
	
	
};
