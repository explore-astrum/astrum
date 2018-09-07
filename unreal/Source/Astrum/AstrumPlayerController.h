// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AstrumPlayerController.generated.h"

/**
 * 
 */
UCLASS(SpatialType)
class ASTRUM_API AAstrumPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void InitPlayerState() override;

	public:
		UPROPERTY(EditAnywhere)
		FString userID = "1";

		UFUNCTION(BlueprintCallable)
		FString GetUserID();
	
	
	
	
};
